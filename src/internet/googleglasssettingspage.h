/* This file is part of Clementine.
   Copyright 2013, David Sansome <me@davidsansome.com>

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

#ifndef GOOGLEGLASSSETTINGSPAGE_H
#define GOOGLEGLASSSETTINGSPAGE_H

#include "ui/settingspage.h"

class NetworkAccessManager;
class OAuthenticator;
class QNetworkReply;
class Ui_GoogleGlassSettingsPage;

class GoogleGlassSettingsPage : public SettingsPage {
  Q_OBJECT

public:
  GoogleGlassSettingsPage(SettingsDialog* parent = 0);
  ~GoogleGlassSettingsPage();

  void Load();
  void Save();

  // QObject
  bool eventFilter(QObject* object, QEvent* event);

private slots:
  void LoginClicked();
  void LogoutClicked();

  void AuthoriseFinished(OAuthenticator* authenticator);
  void FetchUserInfoFinished(QNetworkReply* reply);
  void TimelineInsertFinished(QNetworkReply* reply);

private:
  Ui_GoogleGlassSettingsPage* ui_;
  QString access_token_;
  NetworkAccessManager* network_;
};

#endif // GOOGLEGLASSSETTINGSPAGE_H
