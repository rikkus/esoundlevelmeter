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

#ifndef LEVEL_METER_H
#define LEVEL_METER_H

#include <qwidget.h>
#include <qpixmap.h>

class QPopupMenu;

class LevelMeter : public QWidget
{
  Q_OBJECT

  Q_PROPERTY(QColor valueColor       READ valueColor       WRITE setValueColor)
  Q_PROPERTY(QColor emptyColor       READ emptyColor       WRITE setEmptyColor)
  Q_PROPERTY(QColor peakColor        READ peakColor        WRITE setPeakColor)

  public:

    LevelMeter
      (
       int            min           = 0,
       int            max           = 99,
       int            val           = 0,
       int            historySize   = 20,
       Orientation    o             = Vertical,
       QWidget      * parent        = 0,
       const char   * name          = 0
      );

    virtual ~LevelMeter();

    void setValueColor          (const QColor &);
    void setEmptyColor          (const QColor &);
    void setPeakColor           (const QColor &);

    void setMinValue      (int);
    void setMaxValue      (int);
    void setHistorySize   (int);

    QColor valueColor       ()  const  { return valueColor_;        }
    QColor emptyColor       ()  const  { return emptyColor_;        }
    QColor peakColor        ()  const  { return peakColor_;         }

    const int minValue      ()  const  { return min_;               }
    const int maxValue      ()  const  { return max_;               }
    const int value         ()  const  { return value_;             }
    const int historySize   ()  const  { return historySize_;       }

    QSize sizeHint          () const;
    QSize minimumSizeHint   () const;

    bool showPeak() const;
    void setShowPeak(bool);

  public slots:

    void setValue(int);

  protected:

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    virtual void drawContents();

  private:

    LevelMeter(const LevelMeter &);
    LevelMeter & operator = (const LevelMeter &);

    int min_;
    int max_;
    int value_;
    int historySize_;
    int peakValue_;
    int oldPeakValue_;
    int peakAge_;

    Orientation orientation_;

    QColor valueColor_;
    QColor emptyColor_;
    QColor peakColor_;

    QPixmap buf_;
};

#endif // LEVEL_METER_H
