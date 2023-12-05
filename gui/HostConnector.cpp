/*
    KSysGuard, the KDE System Guard

    Copyright (c) 1999, 2000 Chris Schlaeger <cs@kde.org>

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

#include <KAcceleratorManager>
#include <KComboBox>
#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QGridLayout>
#include <QLineEdit>

#include "HostConnector.h"

HostConnector::HostConnector(QWidget *parent, const QString &name )
  : QDialog( parent )
{
  setObjectName( name );
  setModal( true );
  setWindowTitle( i18n( "Connect Host" ) );

  QFrame *page = new QFrame( this );
  QVBoxLayout *vlayout = new QVBoxLayout(this);
  mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  vlayout->addWidget(page);
  vlayout->addWidget(mButtonBox);
  setLayout(vlayout);

  QGridLayout *layout = new QGridLayout( page );
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->setColumnStretch( 1, 1 );

  QLabel *label = new QLabel( i18n( "Host:" ), page );
  layout->addWidget( label, 0, 0 );

  mHostNames = new KComboBox( true, page );
  mHostNames->setMaxCount( 20 );
  mHostNames->setInsertPolicy( QComboBox::InsertAtTop );
  mHostNames->setAutoCompletion( true );
  mHostNames->setDuplicatesEnabled( false );
  layout->addWidget( mHostNames, 0, 1 );
  label->setBuddy( mHostNames );
  mHostNames->setWhatsThis( i18n( "Enter the name of the host you want to connect to." ) );

  mHostNameLabel = new QLabel( page );
  mHostNameLabel->hide();
  layout->addWidget( mHostNameLabel, 0, 1 );

  QGroupBox *group = new QGroupBox(i18n( "Connection Type" ), page );
  QGridLayout *groupLayout = new QGridLayout();
  group->setLayout(groupLayout);
  groupLayout->setAlignment( Qt::AlignTop );

  mUseSsh = new QRadioButton( i18n( "ssh" ));
  mUseSsh->setEnabled( true );
  mUseSsh->setChecked( true );
  mUseSsh->setWhatsThis( i18n( "Select this to use the secure shell to login to the remote host." ) );
  groupLayout->addWidget( mUseSsh, 0, 0 );

  mUseRsh = new QRadioButton( i18n( "rsh" ));
  mUseRsh->setWhatsThis( i18n( "Select this to use the remote shell to login to the remote host." ) );
  groupLayout->addWidget( mUseRsh, 0, 1 );

  mUseDaemon = new QRadioButton( i18n( "Daemon" ));
  mUseDaemon->setWhatsThis( i18n( "Select this if you want to connect to a ksysguard daemon that is running on the machine you want to connect to, and is listening for client requests." ) );
  groupLayout->addWidget( mUseDaemon, 0, 2 );

  mUseCustom = new QRadioButton( i18n( "Custom command" ));
  mUseCustom->setWhatsThis( i18n( "Select this to use the command you entered below to start ksysguardd on the remote host." ) );
  groupLayout->addWidget( mUseCustom, 0, 3 );

  label = new QLabel( i18n( "Port:" ));
  groupLayout->addWidget( label, 1, 0 );

  mPort = new QSpinBox();
  mPort->setRange( 1, 65535 );
  mPort->setEnabled( false );
  mPort->setValue( 3112 );
  mPort->setToolTip( i18n( "Enter the port number on which the ksysguard daemon is listening for connections." ) );
  groupLayout->addWidget( mPort, 1, 2 );

  label = new QLabel( i18n( "e.g.  3112" ));
  groupLayout->addWidget( label, 1, 3 );

  label = new QLabel( i18n( "Command:" ) );
  groupLayout->addWidget( label, 2, 0 );

  mCommands = new KComboBox( true );
  mCommands->setEnabled( false );
  mCommands->setMaxCount( 20 );
  mCommands->setInsertPolicy( QComboBox::InsertAtTop );
  mCommands->setAutoCompletion( true );
  mCommands->setDuplicatesEnabled( false );
  mCommands->setWhatsThis( i18n( "Enter the command that runs ksysguardd on the host you want to monitor." ) );
  groupLayout->addWidget( mCommands, 2, 2, 1, 2 );
  label->setBuddy( mCommands );

  label = new QLabel( i18n( "e.g. ssh -l root remote.host.org ksysguardd" ) );
  groupLayout->addWidget( label, 3, 2, 1, 2 );

  layout->addWidget( group, 1, 0, 1, 2 );

  connect(mButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(mButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  connect(mUseCustom, &QRadioButton::toggled, mCommands, &KComboBox::setEnabled);
  connect(mUseDaemon, &QRadioButton::toggled, mPort, &QSpinBox::setEnabled);
  connect( mHostNames->lineEdit(),  &QLineEdit::textChanged,
           this, &HostConnector::slotHostNameChanged );
  mButtonBox->button(QDialogButtonBox::Ok)->setEnabled( !mHostNames->lineEdit()->text().isEmpty() );
  KAcceleratorManager::manage( this );
}

HostConnector::~HostConnector()
{
}

void HostConnector::slotHostNameChanged( const QString &_text )
{
    mButtonBox->button(QDialogButtonBox::Ok)->setEnabled( !_text.isEmpty() );
}

void HostConnector::setHostNames( const QStringList &list )
{
  mHostNames->addItems( list );
}

QStringList HostConnector::hostNames() const
{
  QStringList list;

	for ( int i = 0; i < mHostNames->count(); ++i )
    list.append( mHostNames->itemText( i ) );

  return list;
}

void HostConnector::setCommands( const QStringList &list )
{
  mCommands->addItems( list );
}

QStringList HostConnector::commands() const
{
  QStringList list;

	for ( int i = 0; i < mCommands->count(); ++i )
    list.append( mCommands->itemText( i ) );

  return list;
}

void HostConnector::setCurrentHostName( const QString &hostName )
{
  if ( !hostName.isEmpty() ) {
    mHostNames->hide();
    mHostNameLabel->setText( hostName );
    mHostNameLabel->show();
    mButtonBox->button(QDialogButtonBox::Ok)->setEnabled( true );//enable true when mHostNames is empty and hidden fix #66955
  } else {
    mHostNameLabel->hide();
    mHostNames->show();
    mHostNames->setFocus();
  }
}

QString HostConnector::currentHostName() const
{
  return mHostNames->currentText();
}

QString HostConnector::currentCommand() const
{
  return mCommands->currentText();
}

int HostConnector::port() const
{
  return mPort->value();
}

bool HostConnector::useSsh() const
{
  return mUseSsh->isChecked();
}

bool HostConnector::useRsh() const
{
  return mUseRsh->isChecked();
}

bool HostConnector::useDaemon() const
{
  return mUseDaemon->isChecked();
}

bool HostConnector::useCustom() const
{
  return mUseCustom->isChecked();
}


