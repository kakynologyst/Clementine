/* This file is part of Clementine.
   Copyright 2003, Stanislav Karchebny <berkus@users.sf.net>
   Copyright 2003, Max Howell <max.howell@methylblue.com>
   Copyright 2009-2010, David Sansome <davidsansome@gmail.com>
   Copyright 2014, Mark Furneaux <mark@romaco.ca>
   Copyright 2014, Krzysztof Sobiecki <sobkas@gmail.com>
   Copyright 2014, John Maguire <john.maguire@gmail.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Original Author:  Stanislav Karchebny  <berkus@users.sf.net>   2003
 * Original Author:  Max Howell  <max.howell@methylblue.com>  2003 
 */

#include <cmath>
#include <QPainter>

#include "turbine.h"

using Analyzer::Scope;

const char* TurbineAnalyzer::kName =
    QT_TRANSLATE_NOOP("AnalyzerContainer", "Turbine");

void TurbineAnalyzer::analyze(QPainter& p, const Scope& scope, bool new_frame) {
  if (!new_frame) {
    p.drawPixmap(0, 0, canvas_);
    return;
  }

  float h;
  const uint hd2 = height() / 2;
  const uint MAX_HEIGHT = hd2 - 1;

  QPainter canvas_painter(&canvas_);
  canvas_.fill(palette().color(QPalette::Background));

  for (uint i = 0, x = 0, y; i < BAND_COUNT; ++i, x += COLUMN_WIDTH + 1) {
    h = log10(scope[i] * 256.0) * F * 0.5;

    if (h > MAX_HEIGHT) h = MAX_HEIGHT;

    if (h > bar_height[i]) {
      bar_height[i] = h;

      if (h > peak_height[i]) {
        peak_height[i] = h;
        peak_speed[i] = 0.01;
      } else {
        goto peak_handling;
      }
    } else {
      if (bar_height[i] > 0.0) {
        bar_height[i] -= K_barHeight;  // 1.4
        if (bar_height[i] < 0.0) bar_height[i] = 0.0;
      }

    peak_handling:

      if (peak_height[i] > 0.0) {
        peak_height[i] -= peak_speed[i];
        peak_speed[i] *= F_peakSpeed;  // 1.12

        if (peak_height[i] < bar_height[i]) peak_height[i] = bar_height[i];
        if (peak_height[i] < 0.0) peak_height[i] = 0.0;
      }
    }

    y = hd2 - static_cast<uint>(bar_height[i]);
    canvas_painter.drawPixmap(x + 1, y, barPixmap, 0, y, -1, -1);
    canvas_painter.drawPixmap(x + 1, hd2, barPixmap, 0,
                              static_cast<int>(bar_height[i]),
                              -1, -1);

    canvas_painter.setPen(palette().color(QPalette::Highlight));
    if (bar_height[i] > 0)
      canvas_painter.drawRect(x, y, COLUMN_WIDTH - 1,
                              static_cast<int>(bar_height[i]) * 2 - 1);

    const uint x2 = x + COLUMN_WIDTH - 1;
    canvas_painter.setPen(palette().color(QPalette::Base));
    y = hd2 - uint(peak_height[i]);
    canvas_painter.drawLine(x, y, x2, y);
    y = hd2 + uint(peak_height[i]);
    canvas_painter.drawLine(x, y, x2, y);
  }

  p.drawPixmap(0, 0, canvas_);
}
