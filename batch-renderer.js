const { Worker } = require('worker_threads');
const os = require('os');
const path = require('path');
const fs = require('fs').promises;

class VitaBatchRenderer {
    constructor(options = {}) {
        this.numWorkers = options.workers || os.cpus().length;
        this.defaultParams = {
            note: 60,
            velocity: 0.7,
            noteDuration: 2.0,
            renderDuration: 4.0,
            ...options.defaultParams
        };
    }

    /**
     * Render multiple presets in parallel
     */
    async renderBatch(presetPaths, outputPaths, renderParams = {}) {
        if (!Array.isArray(presetPaths) || presetPaths.length === 0) {
            throw new Error('presetPaths must be a non-empty array');
        }

        const params = { ...this.defaultParams, ...renderParams };
        const resolvedOutputPaths = await this._resolveOutputPaths(presetPaths, outputPaths);
        
        // Create jobs with all necessary info
        const jobs = presetPaths.map((presetPath, index) => ({
            id: index,
            presetPath,
            outputPath: resolvedOutputPaths[index],
            params
        }));

        // Process jobs using worker pool
        const workers = await this._createWorkerPool();
        const results = await this._processJobs(jobs, workers);
        
        // Cleanup
        await Promise.all(workers.map(w => w.terminate()));
        
        return results;
    }

    /**
     * Render with progress callback
     */
    async renderBatchWithProgress(presetPaths, outputPaths, renderParams, onProgress) {
        // Handle overloaded parameters
        if (typeof renderParams === 'function') {
            onProgress = renderParams;
            renderParams = {};
        }

        const total = presetPaths.length;
        let completed = 0;

        // Wrap the batch renderer with progress tracking
        const progressWrapper = async (jobs, workers) => {
            const results = new Array(jobs.length);
            
            for (const job of jobs) {
                const worker = await this._getAvailableWorker(workers);
                const result = await this._executeJob(worker, job);
                
                results[job.id] = result;
                completed++;
                
                if (onProgress) {
                    onProgress({
                        completed,
                        total,
                        percentage: (completed / total) * 100,
                        current: result
                    });
                }
            }
            
            return results;
        };

        const params = { ...this.defaultParams, ...renderParams };
        const resolvedOutputPaths = await this._resolveOutputPaths(presetPaths, outputPaths);
        
        const jobs = presetPaths.map((presetPath, index) => ({
            id: index,
            presetPath,
            outputPath: resolvedOutputPaths[index],
            params
        }));

        const workers = await this._createWorkerPool();
        const results = await progressWrapper(jobs, workers);
        
        await Promise.all(workers.map(w => w.terminate()));
        
        return results;
    }

    /**
     * Resolve output paths
     */
    async _resolveOutputPaths(presetPaths, outputPaths) {
        // Array of paths provided
        if (Array.isArray(outputPaths)) {
            if (outputPaths.length !== presetPaths.length) {
                throw new Error(`Output paths length (${outputPaths.length}) must match preset paths length (${presetPaths.length})`);
            }
            return outputPaths;
        }
        
        // Single directory provided
        if (typeof outputPaths === 'string') {
            await fs.mkdir(outputPaths, { recursive: true });
            
            const nameCount = new Map();
            return presetPaths.map(presetPath => {
                const baseName = path.basename(presetPath, path.extname(presetPath));
                const count = nameCount.get(baseName) || 0;
                nameCount.set(baseName, count + 1);
                
                const fileName = count === 0 ? `${baseName}.wav` : `${baseName}_${count}.wav`;
                return path.join(outputPaths, fileName);
            });
        }
        
        throw new Error('outputPaths must be either a string (directory) or array of strings (file paths)');
    }

    /**
     * Create worker pool
     */
    async _createWorkerPool() {
        const workers = [];
        const workerPath = path.join(__dirname, 'vita-worker.js');
        
        // Create all workers and wait for them to be ready
        const initPromises = Array(this.numWorkers).fill().map((_, i) => {
            return new Promise((resolve, reject) => {
                const worker = new Worker(workerPath, {
                    workerData: { workerId: i }
                });
                
                worker.busy = false;
                worker.resolver = null;
                
                // Setup message handler
                worker.on('message', (msg) => {
                    if (msg.type === 'ready') {
                        workers.push(worker);
                        resolve();
                    } else if (worker.resolver) {
                        worker.resolver(msg);
                        worker.resolver = null;
                        worker.busy = false;
                    }
                });
                
                worker.on('error', (error) => {
                    if (worker.resolver) {
                        worker.resolver({ type: 'error', error: error.message });
                        worker.resolver = null;
                    }
                    worker.busy = false;
                });
                
                // Timeout if worker doesn't initialize
                setTimeout(() => reject(new Error(`Worker ${i} failed to initialize`)), 10000);
            });
        });
        
        await Promise.all(initPromises);
        return workers;
    }

    /**
     * Process all jobs using worker pool
     */
    async _processJobs(jobs, workers) {
        const results = new Array(jobs.length);
        const queue = [...jobs];
        
        // Process jobs in parallel
        const processPromises = workers.map(async (worker) => {
            while (queue.length > 0) {
                const job = queue.shift();
                if (!job) break;
                
                const result = await this._executeJob(worker, job);
                results[job.id] = result;
            }
        });
        
        await Promise.all(processPromises);
        return results;
    }

    /**
     * Get next available worker
     */
    async _getAvailableWorker(workers) {
        while (true) {
            const worker = workers.find(w => !w.busy);
            if (worker) return worker;
            
            // Wait a bit before checking again
            await new Promise(resolve => setTimeout(resolve, 10));
        }
    }

    /**
     * Execute single job on worker
     */
    async _executeJob(worker, job) {
        return new Promise((resolve) => {
            worker.busy = true;
            worker.resolver = resolve;
            worker.postMessage({ type: 'render', job });
        }).then(msg => {
            if (msg.type === 'complete') {
                return {
                    success: true,
                    presetPath: job.presetPath,
                    outputPath: job.outputPath,
                    duration: msg.duration,
                    fileSize: msg.fileSize
                };
            } else {
                return {
                    success: false,
                    presetPath: job.presetPath,
                    outputPath: job.outputPath,
                    error: msg.error
                };
            }
        });
    }
}

module.exports = VitaBatchRenderer;