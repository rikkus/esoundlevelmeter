// vim:tabstop=2:shiftwidth=2:expandtab:cinoptions=(s,U1,m1
/*
  Copyright 2002 Rik Hemsley (rikkus) <rik@kde.org>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <qpainter.h>

#include "levelmeter.h"

LevelMeter::LevelMeter
(
  int min,
  int max,
  int val,
  int historySize,
  Orientation o,
  QWidget * parent,
  const char * name
)
  : QWidget(parent, name, WRepaintNoErase | WResizeNoErase | WPaintUnclipped),
    min_                (min),
    max_                (max),
    value_              (val),
    historySize_        (historySize),
    peakValue_          (0),
    oldPeakValue_       (0),
    orientation_        (o),
    valueColor_         (black),
    emptyColor_         (QColor(215, 242, 42)),
    peakColor_          (black)
{
  setBackgroundMode(NoBackground);

  if (orientation_ == Horizontal)
    setSizePolicy
      (QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
  else
    setSizePolicy
      (QSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding));

  if (min_ >= max_)
  {
    qWarning("LevelMeter ctor: min value is >= max !");
    qWarning("Setting min to 0 and max to 99");
    min_ = 0;
    max_ = 99;
  }

  if (value_ < min_)
  {
    qWarning("LevelMeter ctor: value is < min - clipping");
    value_ = min_;
  }
  else if (value_ > max_)
  {
    qWarning("LevelMeter ctor: value is > max - clipping");
    value_ = min_;
  }

  buf_.resize(size());
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::setMinValue(int i)
{
  if (i > max_)
  {
    qWarning("LevelMeter::setMinValue(%d): value is > max - clipping", i);
    min_ = max_ - 1;
  }
  else
  {
    min_ = i;
  }

  drawContents();
}

void LevelMeter::setMaxValue(int i)
{
  if (i < max_)
  {
    qWarning("LevelMeter::setMaxValue(%d): value is < min - clipping", i);
    min_ = max_ - 1;
  }
  max_ = i;
  drawContents();
}

void LevelMeter::setValue(int i)
{
  if (i < min_)
  {
    qWarning("LevelMeter::setValue(%d): clipping to %d", i, min_);
    i = min_;
  }
  else if (i > max_)
  {
    qWarning("LevelMeter::setValue(%d): clipping to %d", i, max_);
    i = max_;
  }

  if (value_ != i)
  {
    value_ = i;

    if (value_ > peakValue_)
      peakValue_ = value_;

    if (peakValue_ > oldPeakValue_ || peakAge_ > historySize_)
    {
      oldPeakValue_ = peakValue_;
      peakValue_ = 0;
      peakAge_ = 0;
    }
    else
    {
      ++peakAge_;
    }
  }

  drawContents();
}

void LevelMeter::setValueColor(const QColor & c)
{
  valueColor_ = c;
  drawContents();
}

void LevelMeter::setEmptyColor(const QColor & c)
{
  emptyColor_ = c;
  drawContents();
}

void LevelMeter::setPeakColor(const QColor & c)
{
  peakColor_ = c;
  drawContents();
}

void LevelMeter::drawContents()
{
  QPainter p(&buf_);

  const uint border = 4;
  const uint cellSize = 3;

  p.fillRect(rect(), emptyColor_);

  uint l = width() - border * 2 - cellSize * 2;

  uint valueEnd = uint((value_ / float(max_)) * l);
  QColor noValueColor = emptyColor_.dark(120);

  for (uint i = 0; i < l; i += cellSize * 2)
  {
    p.fillRect
      (
        border + cellSize + i,
        border,
        cellSize,
        height() - border * 2,
        i < valueEnd ? valueColor_ : noValueColor
      );
  }

  uint peak = uint((oldPeakValue_ / float(max_)) * l) / (cellSize*2) *
    (cellSize*2);

  p.fillRect
    (
      border + cellSize + peak,
      border,
      cellSize,
      height() - border * 2,
      valueColor_
    );

  bitBlt(this, 0, 0, &buf_);
}

void LevelMeter::paintEvent(QPaintEvent *)
{
  drawContents();
}

void LevelMeter::resizeEvent(QResizeEvent *)
{
  buf_.resize(size());
  drawContents();
}

QSize LevelMeter::sizeHint() const
{
  if (orientation_ == Horizontal)
    return QSize(128, 16);
  else
    return QSize(16, 128);
}

QSize LevelMeter::minimumSizeHint() const
{
  if (orientation_ == Horizontal)
    return QSize(16, 2);
  else
    return QSize(2, 16);
}

