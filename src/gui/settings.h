﻿#ifndef SETTINGS_H
#define SETTINGS_H
/****************************************************************************
** RDM - Reconhecimento Digital de Matriculas
** Copyright (C) 2011  Duarte Duarte, Miguel Mendes
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
** 
** We are not to be held liable for any illegal use of this software.
** 
****************************************************************************/

#include <QWidget>

class settingss : public QWidget
{
    Q_OBJECT
public:
    explicit settingss(QWidget *parent = 0);
};

#endif // SETTINGS_H
