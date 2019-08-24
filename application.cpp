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

#include <qsettings.h>
#include <qmainwindow.h>

#include "mainwidget.h"
#include "application.h"

Application::Application(int argc, char ** argv)
	:	QApplication(argc, argv)
{
	mainWindow_ = new QMainWindow;

	setMainWidget(mainWindow_);

	mainWindow_->setCaption(tr("esound level meter"));

	MainWidget * mainWidget = new MainWidget(mainWindow_);

  QSettings settings;

  uint w = settings.readNumEntry("/esoundlevelmeter/width", 128);
  uint h = settings.readNumEntry("/esoundlevelmeter/height", 64);

  mainWindow_->resize(w, h);

	mainWindow_->setCentralWidget(mainWidget);

	mainWindow_->show();
}

Application::~Application()
{
  QSettings settings;

  settings.writeEntry("/esoundlevelmeter/width", mainWindow_->width());
  settings.writeEntry("/esoundlevelmeter/height", mainWindow_->height());

  delete mainWindow_;
}

