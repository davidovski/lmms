#ifndef SINGLE_SOURCE_COMPILE

/*
 * controller.cpp - implementation of class controller which handles remote-control
 *                  of automatableModels
 *
 * Copyright (c) 2008 Paul Giblock <drfaygo/at/gmail.com>
 *
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include <Qt/QtXml>
#include <QtCore/QObject>
#include <QtCore/QVector>


#include "song.h"
#include "engine.h"
#include "mixer.h"
#include "controller.h"
#include "controller_dialog.h"


unsigned int controller::s_frames = 0;
QVector<controller *> controller::s_controllers;

controller::controller( model * _parent ) :
	model( _parent )
{
	s_controllers.append( this );
}


controller::~controller()
{
	s_controllers.remove( s_controllers.indexOf( this ) );

}


// Get current value, with an offset into the current buffer for sample exactness
float controller::currentValue( int _offset )
{
	if( _offset == 0 || isSampleExact() )
	{
		m_currentValue = fittedValue( value( _offset ) );
	}
	
	return m_currentValue;
}


float controller::value( int _offset )
{
	return 0.5f;
}
	


// Get position in frames
int controller::runningFrames()
{
	return s_frames;
}

// Get position in seconds
float controller::runningTime()
{
	return s_frames /
			engine::getMixer()->sampleRate() ;
}


void controller::triggerFrameCounter( void )
{
	for( int i = 0; i < s_controllers.size(); ++i ) 
	{
		emit s_controllers.at(i)->valueChanged();
	}

	s_frames += engine::getMixer()->framesPerPeriod();
	//emit s_signaler.triggerValueChanged();
}

void controller::resetFrameCounter( void )
{
	s_frames = 0;
}

controllerDialog * controller::createDialog( QWidget * _parent )
{
	controllerDialog * d = new controllerDialog( this, _parent );

	return d;
}


#include "controller.moc"


#endif
