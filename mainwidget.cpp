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

#include <qtimer.h>
#include <qprocess.h>
#include <qlayout.h>
#include <qcursor.h>
#include <qcolordialog.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qsettings.h>

#include "mainwidget.h"
#include "levelmeter.h"

MainWidget::MainWidget(QWidget * parent, const char * name)
  : QWidget(parent, name),
    valueColor_         (black),
    emptyColor_         (QColor(215, 242, 42)),
    peakColor_          (black)
{
  QSettings settings;

  valueColor_ =
    QColor
    (
      settings.readEntry
      ("/esoundlevelmeter/foregroundcolor",
        valueColor_.name()
      )
    );

  emptyColor_ =
    QColor
    (
      settings.readEntry
      (
        "/esoundlevelmeter/backgroundcolor",
        emptyColor_.name()
      )
    );

  peakColor_ =
    QColor
    (
      settings.readEntry
      (
        "/esoundlevelmeter/peakcolor",
        peakColor_.name()
      )
    );

  popup_ = new QPopupMenu(this);

  popup_->insertItem
    (
      tr("Set background colour..."),
      this,
      SLOT(slotChooseBackgroundColour())
    );

  popup_->insertItem
    (
      tr("Set foreground colour..."),
      this,
      SLOT(slotChooseForegroundColour())
    );

	setBackgroundColor(QColor(215, 242, 42));

	levelMeter1_ =
		new LevelMeter(0, 99, 95, 20, Qt::Horizontal, this, "meter 1");

	levelMeter2_ =
		new LevelMeter(0, 99, 95, 30, Qt::Horizontal, this, "meter 2");

  setColors();

	QVBoxLayout * layout = new QVBoxLayout(this);

	layout->addSpacing(4);
	layout->addWidget(levelMeter1_);
	layout->addWidget(levelMeter2_);
	layout->addSpacing(4);

	timer_ = new QTimer(this);

	connect(timer_, SIGNAL(timeout()), SLOT(slotUpdate()));

	timer_->start(100);

	process_ = new QProcess(QString("esdmon"), this, "esdmon process");

	connect(process_, SIGNAL(readyReadStdout()), SLOT(slotReadyReadStdout()));
	connect(process_, SIGNAL(processExited()), SLOT(slotProcessExited()));

	process_->start();
}

void MainWidget::mousePressEvent(QMouseEvent * e)
{
  if (RightButton == e->button())
  {
    popup_->exec(QCursor::pos());
  }

  QWidget::mousePressEvent(e);
}

void MainWidget::slotChooseBackgroundColour()
{
  QColor c = QColorDialog::getColor(emptyColor_, this);

  if (!c.isValid())
    return;

  emptyColor_ = c;
  setColors();
}

void MainWidget::slotChooseForegroundColour()
{
  QColor c = QColorDialog::getColor(valueColor_, this);

  if (!c.isValid())
    return;

  valueColor_ = c;
  setColors();
}

void MainWidget::slotChoosePeakColour()
{
  QColor c = QColorDialog::getColor(peakColor_, this);

  if (!c.isValid())
    return;

  peakColor_ = c;
  setColors();
}

void MainWidget::slotReadyReadStdout()
{
	buffer_ = process_->readStdout();
}

void MainWidget::slotUpdate()
{
	if (0 != buffer_.size() % 4)
	{
		qDebug("Hmm, not 4 bytes aligned");
		return;
	}

	Q_UINT16 * data = reinterpret_cast<Q_UINT16 *>(buffer_.data());

	Q_UINT16 lavg = 0;
	Q_UINT16 ravg = 0;

#if 0
	uint lclipCount;
	uint rclipCount;

	bool lclip = false;
	bool rclip = false;
#endif

	for (uint i = 0; i < buffer_.size() / 2; i += 2)
	{
		Q_UINT16 l = data[i];
		Q_UINT16 r = data[i + 1];

		lavg = uint(((99.0 * lavg) + l) / 100.0);
		ravg = uint(((99.0 * ravg) + r) / 100.0);

#if 0
		if (65535 == l)
			++lclipCount;
		else
			lclipCount = 0;

		if (65535 == r)
			++rclipCount;
		else
			rclipCount = 0;

		if (lclipCount > 1)
			lclip = true;

		if (rclipCount > 1)
			rclip = true;
#endif
	}

	lavg = uint((lavg / 65535.f) * 99);
	ravg = uint((ravg / 65535.f) * 99);

	levelMeter1_->setValue(lavg);
	levelMeter2_->setValue(ravg);

//	levelMeter1_->setClipping(lclip);
//	levelMeter2_->setClipping(rclip);
}

	void
MainWidget::slotProcessExited()
{
	qWarning("esdmon exited");
	qApp->quit();
}


void MainWidget::setColors()
{
  levelMeter1_->setValueColor       (valueColor_);
  levelMeter1_->setEmptyColor       (emptyColor_);
  levelMeter1_->setPeakColor        (peakColor_);
  levelMeter2_->setValueColor       (valueColor_);
  levelMeter2_->setEmptyColor       (emptyColor_);
  levelMeter2_->setPeakColor        (peakColor_);

	setBackgroundColor(emptyColor_);
  update();

  QSettings settings;

  settings.writeEntry("/esoundlevelmeter/foregroundcolor", valueColor_.name());
  settings.writeEntry("/esoundlevelmeter/backgroundcolor", emptyColor_.name());
  settings.writeEntry("/esoundlevelmeter/peakcolor", peakColor_.name());
}
