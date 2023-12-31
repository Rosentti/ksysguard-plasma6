/*
    KSysGuard, the KDE System Guard
   
    Copyright (c) 1999 - 2002 Chris Schlaeger <cs@kde.org>
    
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License or (at your option) version 3 or any later version
 accepted by the membership of KDE e.V. (or its successor approved
 by the membership of KDE e.V.), which shall act as a proxy 
 defined in Section 14 of version 3 of the license.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KSG_WORKSHEET_H
#define KSG_WORKSHEET_H

#include <QWidget>
#include <QTimer>
#include <QStringList>

#include <SensorDisplay.h>
#include "SharedSettings.h"

class QDomElement;
class QDragEnterEvent;
class QDropEvent;
class QGridLayout;

/**
  A WorkSheet contains the displays to visualize the sensor results. When
  creating the WorkSheet you must specify the number of columns. Displays
  can be added and removed on the fly. The grid layout will handle the
  layout. The number of columns can not be changed. Displays are added by
  dragging a sensor from the sensor browser over the WorkSheet.
 */
class WorkSheet : public QWidget
{
  Q_OBJECT

  public:
    explicit WorkSheet( QWidget* parent);
    WorkSheet( int rows, int columns, float interval, QWidget* parent);
    ~WorkSheet() override;

    bool load( const QString &fileName );
    bool save( const QString &fileName );
    bool exportWorkSheet( const QString &fileName );

    void cut();
    void copy();
    void paste();

    void setFileName( const QString &fileName );
    QString fileName() const;

    QString fullFileName() const;

    bool isLocked() const {return mSharedSettings.locked;}

    QString title() const;
    QString translatedTitle() const;
    void refreshSheet();

    KSGRD::SensorDisplay* addDisplay( const QString &hostname,
                                      const QString &monitor,
                                      const QString &sensorType,
                                      const QString &sensorDescr,
                                      int row, int column );

    void settings();
    float updateInterval() const;

  public Q_SLOTS:
    void showPopupMenu( KSGRD::SensorDisplay *display );
    void setTitle( const QString &title );
    void applyStyle();

  Q_SIGNALS:
    void titleChanged( QWidget *sheet );

  protected:

    void changeEvent( QEvent * event ) override;
    QSize sizeHint() const override;
    void dragMoveEvent( QDragMoveEvent* ) override;
    void dragEnterEvent( QDragEnterEvent* ) override;
    void dropEvent( QDropEvent* ) override;
    bool event( QEvent* ) override;
    void setUpdateInterval( float interval);

  private:
    void removeDisplay( KSGRD::SensorDisplay *display );

    bool replaceDisplay( int row, int column, QDomElement& element, int rowSpan = 1, int columnSpan = 1 );

    void replaceDisplay( int row, int column, KSGRD::SensorDisplay* display = nullptr, int rowSpan = 1, int columnSpan = 1 );

    void collectHosts( QStringList &list );

    void createGrid( int rows, int columns );

    void resizeGrid( int rows, int columns );

    KSGRD::SensorDisplay* currentDisplay( int *row = nullptr, int *column = nullptr );

    void fixTabOrder();

    QString currentDisplayAsXML();

    int mRows;
    int mColumns;

    QGridLayout* mGridLayout;
    QString mFileName;
    QString mFullFileName;
    QString mTitle;
    QString mTranslatedTitle;

    SharedSettings mSharedSettings;

    QTimer mTimer;

    enum DisplayType { DisplayDummy, DisplayFancyPlotter, DisplayMultiMeter, DisplayDancingBars, DisplaySensorLogger, DisplayListView, DisplayLogFile, DisplayProcessControllerRemote, DisplayProcessControllerLocal };

    KSGRD::SensorDisplay* insertDisplay( DisplayType displayType, QString displayTitle, int row, int column, int rowSpan = 1, int columnSpan = 1);
};

#endif
