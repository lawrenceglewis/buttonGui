/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <QColor>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "rviz/visualization_manager.h"
#include "rviz/render_panel.h"
#include "rviz/display.h"


#include "myviz.h"
#include <std_srvs/Trigger.h>

#include "cstdlib"

#include <ros/ros.h>



// Constructor for MyViz.  This does most of the work of the class.
MyViz::MyViz( QWidget* parent )
  : rviz::Panel( parent )
{
  // Construct and lay out labels and slider controls.
  QLabel* thickness_label = new QLabel( "Line Thickness" );
  QLabel* file_name_label = new QLabel( "Insert File Here");
  //QPushButton* call_service_btn_1 = new QPushButton( "Call WristCalSrvStart" , parent );
  QPushButton* call_service_btn_2 = new QPushButton( "Capture" , parent );
  QPushButton* call_service_btn_3 = new QPushButton( "Execute" , parent );
  QPushButton* call_service_btn_4 = new QPushButton( "ExecuteWCall" , parent );
  QPushButton* call_service_btn_5 = new QPushButton( "MoveEnd" , parent );
  QPushButton* call_service_btn_6 = new QPushButton( "MoveNext" , parent );
  QPushButton* call_service_btn_7 = new QPushButton( "MovePrev" , parent );
  QPushButton* call_service_btn_8 = new QPushButton( "MoveStart" , parent );
  file_name_text_ = new QLineEdit(parent);
  QSlider* thickness_slider = new QSlider( Qt::Horizontal );
  thickness_slider->setMinimum( 1 );
  thickness_slider->setMaximum( 200 );
  QLabel* cell_size_label = new QLabel( "Cell Size" );
  QSlider* cell_size_slider = new QSlider( Qt::Horizontal );
  cell_size_slider->setMinimum( 1 );
  cell_size_slider->setMaximum( 100 );
  QGridLayout* controls_layout = new QGridLayout();
  //controls_layout->addWidget( file_name_label, 0, 0 );
  //controls_layout->addWidget( file_name_text_, 0, 1);
  controls_layout->addWidget( call_service_btn_2, 0, 0 );
  controls_layout->addWidget( call_service_btn_3, 1, 0 );
  controls_layout->addWidget( call_service_btn_4, 1, 1 );
  controls_layout->addWidget( call_service_btn_5, 3, 1 );
  controls_layout->addWidget( call_service_btn_6, 2, 0 );
  controls_layout->addWidget( call_service_btn_7, 2, 1 );
  controls_layout->addWidget( call_service_btn_8, 3, 0 );
  //controls_layout->addWidget( thickness_label, 2, 0 );
  //controls_layout->addWidget( thickness_slider, 2, 1 );
  //controls_layout->addWidget( cell_size_label, 3, 0 );
  //controls_layout->addWidget( cell_size_slider, 3, 1 );

  // Construct and lay out render panel.
  render_panel_ = new rviz::RenderPanel();
  QVBoxLayout* main_layout = new QVBoxLayout;
  main_layout->addLayout( controls_layout );
  main_layout->addWidget( render_panel_ );

  // Set the top-level layout for this MyViz widget.
  setLayout( main_layout );

  // Make signal/slot connections.
  connect( thickness_slider, SIGNAL( valueChanged( int )), this, SLOT( setThickness( int )));
  connect( cell_size_slider, SIGNAL( valueChanged( int )), this, SLOT( setCellSize( int )));
  connect( call_service_btn_2, SIGNAL( clicked( bool )), this, SLOT( setbutton2Clicked()));
  connect( call_service_btn_3, SIGNAL( clicked( bool )), this, SLOT( setbutton3Clicked()));
  connect( call_service_btn_4, SIGNAL( clicked( bool )), this, SLOT( setbutton4Clicked()));
  connect( call_service_btn_5, SIGNAL( clicked( bool )), this, SLOT( setbutton5Clicked()));
  connect( call_service_btn_6, SIGNAL( clicked( bool )), this, SLOT( setbutton6Clicked()));
  connect( call_service_btn_7, SIGNAL( clicked( bool )), this, SLOT( setbutton7Clicked()));
  connect( call_service_btn_8, SIGNAL( clicked( bool )), this, SLOT( setbutton8Clicked()));


  // Next we initialize the main RViz classes.
  //
  // The VisualizationManager is the container for Display objects,
  // holds the main Ogre scene, holds the ViewController, etc.  It is
  // very central and we will probably need one in every usage of
  // librviz.
  manager_ = new rviz::VisualizationManager( render_panel_ );
  render_panel_->initialize( manager_->getSceneManager(), manager_ );
  manager_->initialize();
  manager_->startUpdate();

  // Create a Grid display.
  grid_ = manager_->createDisplay( "rviz/Grid", "adjustable grid", true );
  ROS_ASSERT( grid_ != NULL );

  // Configure the GridDisplay the way we like it.
  grid_->subProp( "Line Style" )->setValue( "Billboards" );
  grid_->subProp( "Color" )->setValue( QColor( Qt::yellow ) );

  // Initialize the slider values.
  thickness_slider->setValue( 25 );
  cell_size_slider->setValue( 10 );

  ros::NodeHandle n("~");
  //client1_ = n.serviceClient<std_srvs::Trigger>("WristCalSrvStart");
  client2_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajCapture");
  client3_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajExecute");
  client4_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajExecuteWCall");
  client5_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajMoveEnd");
  client6_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajMoveNext");
  client7_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajMovePrev");
  client8_ = n.serviceClient<std_srvs::Trigger>("/joint_traj/JTrajMoveStart");
}
// Destructor.
MyViz::~MyViz()
{
  delete manager_;
}



//void MyViz::setbutton1Clicked ()
//{
//  ROS_INFO("myfilenameis %s ",file_name_text_->text().toStdString().c_str());
//  std_srvs::Trigger srv;
//  if (client1_.call(srv))
//  {
//    ROS_INFO("Output %s",srv.response.message.c_str());
//  }
//  else
//  {
//    ROS_ERROR("Failed to call service Trigger");
//  }
//}

void MyViz::setbutton2Clicked ()
{
  std_srvs::Trigger srv;
  if (client2_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton3Clicked ()
{
  std_srvs::Trigger srv;
  if (client3_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton4Clicked ()
{
  std_srvs::Trigger srv;
  if (client4_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton5Clicked ()
{
  std_srvs::Trigger srv;
  if (client5_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton6Clicked ()
{
  std_srvs::Trigger srv;
  if (client6_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton7Clicked ()
{
  std_srvs::Trigger srv;
  if (client7_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}

void MyViz::setbutton8Clicked ()
{
  std_srvs::Trigger srv;
  if (client8_.call(srv))
  {
    ROS_INFO("Output %s",srv.response.message.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service Trigger");
  }
}









//TODO remove unessisary code

// This function is a Qt slot connected to a QSlider's valueChanged()
// signal.  It sets the cell size of the grid by changing the grid's
// "Cell Size" Property.
void MyViz::setCellSize( int cell_size_percent )
{
  if( grid_ != NULL )
  {
    grid_->subProp( "Cell Size" )->setValue( cell_size_percent / 10.0f );
  }
}
// This function is a Qt slot connected to a QSlider's valueChanged()
// signal.  It sets the line thickness of the grid by changing the
// grid's "Line Width" property.
void MyViz::setThickness( int thickness_percent )
{
  if( grid_ != NULL )
  {
    grid_->subProp( "Line Style" )->subProp( "Line Width" )->setValue( thickness_percent / 100.0f );
  }
}

//set up button
//ROS_INFO("will it work?");
// BEGIN_TUTORIAL
