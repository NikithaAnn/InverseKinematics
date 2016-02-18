#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_gl=new  NGLScene(this);
  m_ui->s_MainGridLayout->addWidget(m_gl,0,0,2,1);
  connect(m_ui->m_wireframe,SIGNAL(clicked(bool)), m_gl, SLOT(toggleWireframe(bool)));
  connect(m_ui->m_load,SIGNAL(clicked()),m_gl,SLOT(loadFile()));
  connect(m_ui->m_CCD,SIGNAL(clicked()),m_gl,SLOT(doCCDIK()));

}

MainWindow::~MainWindow()
{
    delete m_ui;
}
