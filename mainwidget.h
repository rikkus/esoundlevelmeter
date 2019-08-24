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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <qwidget.h>

class QPopupMenu;
class LevelMeter;
class QProcess;
class QTimer;

class MainWidget : public QWidget
{
  Q_OBJECT

  public:
    
    MainWidget(QWidget * parent = 0, const char * name = 0);

  public slots:

    void slotChooseBackgroundColour();
    void slotChooseForegroundColour();
    void slotChoosePeakColour();

	protected slots:

		void slotReadyReadStdout();
		void slotProcessExited();
    void slotUpdate();

  protected:

    void mousePressEvent(QMouseEvent *);
    void setColors();

  private:

		LevelMeter * levelMeter1_, * levelMeter2_;
    QPopupMenu * popup_;

    QProcess * process_;
    QTimer * timer_;

    QColor valueColor_;
    QColor emptyColor_;
    QColor peakColor_;

    QByteArray buffer_;
};

#endif // MAINWIDGET_H
