/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "phaser_filter_test.h"
#include "phaser_filter.h"

void PhaserFilterTest::runTest() {
  vital::PhaserFilter phaser_filter1(true);
  runInputBoundsTest(&phaser_filter1);

  vital::PhaserFilter phaser_filter2(false);
  runInputBoundsTest(&phaser_filter2);
}

static PhaserFilterTest phaser_filter_test;
