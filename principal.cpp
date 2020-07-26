#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    //Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    //Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    //Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    //Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    mDefine=1;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    //Crear el painter de la ventana principal
    QPainter painter(this);
    //Dibujar la  imagen
    painter.drawImage(0, 0 , *mImagen);
    //Aceptar el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    mPuedeDibujar = true;
    mInicial = event->pos();
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    //Validar si se puede dibujar
    if(mDefine==2){
        mPuedeDibujar = false;

    //Capturar el punto donde se suelta el mouse
    mFinal = event->pos();
    //Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    //Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    //Mostrar el numero de lineas en la barra de estado
    ui->statusbar->showMessage("Número de Líneas: " + QString::number(++mNumLineas));
    //Actualizar la interfaz
    update();
    //actualizar el punto inicial
    mInicial = mFinal;

    }
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    //  Primero que define dibujar lineas
       if(mDefine==1){
           mPuedeDibujar = false;
           // Capturar el punto donde se suelta el mouse
           mFinal = event->pos();
           //Establecer un pincel
           QPen pincel;
           pincel.setColor(mColor);
           pincel.setWidth(mAncho);
           //Dibujar una linea
           mPainter->setPen(pincel);
           mPainter->drawLine(mInicial,mFinal);
           //Contar las lineas
           ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
           //Actualizar
           update();
           // Aceptar el evento
           event->accept();
       }
       //  Tercero que define dibujar rectangulos
       if(mDefine==3){
           mPuedeDibujar = false;
           // Capturar el punto donde se suelta el mouse
           mFinal = event->pos();
           //Establecer un pincel
           QPen pincel;
           pincel.setColor(mColor);
           pincel.setWidth(mAncho);
           QRect rectangulo(mInicial,mFinal);
           // Dibujar un Rectangulo
           mPainter->setPen(pincel);
           mPainter->drawRect(rectangulo);
           //Contar las lineas
           ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
           //Actualizar
           update();
           // Aceptar el vento
           event->accept();
       }
       //  Cuarto que define dibujar circunferencias
       if(mDefine==4){
           mPuedeDibujar = false;
           // Capturar el punto donde se suelta el mouse
           mFinal = event->pos();
           QPen pincel;
           pincel.setColor(mColor);
           pincel.setWidth(mAncho);
           //Establecer una variable circuferencia con los puntos
           QRectF circunferencia(mInicial,mFinal);
           // Dibujar una circunferencia
           mPainter->setPen(pincel);
           mPainter->drawEllipse(circunferencia);
           //Contar las lineas
           ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
           update();
           // Aceptar el evento
           event->accept();
       }
}



void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancgo del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    QString nombreArchivo;

        if(mNumLineas==0){
            this->close();
        }else{

            if(!mImagen->save(nombreArchivo)){
                QMessageBox msgBox;
                msgBox.setText("Imagen no guardada.");
                msgBox.addButton("Guardar", QMessageBox::AcceptRole);
                msgBox.addButton(tr("Cancelar"), QMessageBox::RejectRole);
                msgBox.addButton(tr("Salir sin guardar"), QMessageBox::DestructiveRole);
                int ret = msgBox.exec();
                switch (ret) {
                case QMessageBox::AcceptRole:
                    on_actionGuardar_triggered();
                    break;
                case QMessageBox::RejectRole:
                    break;
                case QMessageBox::DestructiveRole:
                    this->close();
                    break;
                default:
                    break;
                }

            }
        }
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    QString nombreArchivo;
        if(!mImagen->save(nombreArchivo)){
            QMessageBox msgBox;
            msgBox.setText("No se ha guardado la imagen.");
            msgBox.addButton("Guardar", QMessageBox::AcceptRole);
            msgBox.addButton("Cancelar", QMessageBox::RejectRole);
            msgBox.addButton("Nuevo", QMessageBox::DestructiveRole);
            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::AcceptRole:
                on_actionGuardar_triggered();
                break;
            case QMessageBox::RejectRole:
                break;
            case QMessageBox::DestructiveRole:
              mImagen->fill(Qt::white);
                break;
            default:
                break;
            }

        }
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar una imagen",
                                                         QString(),
                                                         "Imágenes(*.png");
    if(!nombreArchivo.isEmpty()){
        if(mImagen->save(nombreArchivo))
           QMessageBox::information(this,
                                    "Guardar imagen",
                                    "Archivo almacenado en: " + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}

void Principal::on_actionLineas_triggered()
{
        ui->actionLibre->setCheckable(false);
        ui->actionRectangulos->setCheckable(false);
        ui->actionCircunferencias->setCheckable(false);

    //Para dibujar lineas
    mDefine=1;
}


void Principal::on_actionLibre_triggered()
{

        ui->actionLineas->setCheckable(false);
        ui->actionRectangulos->setCheckable(false);
        ui->actionCircunferencias->setCheckable(false);

   //Para dibujar libremente
    mDefine=2;
}

void Principal::on_actionRectangulos_triggered()
{
    ui->actionLibre->setCheckable(false);
    ui->actionLineas->setCheckable(false);
    ui->actionCircunferencias->setCheckable(false);

//Para dibujar Rectangulos
mDefine=3;
}

void Principal::on_actionCircunferencias_triggered()
{

        ui->actionLibre->setCheckable(false);
        ui->actionRectangulos->setCheckable(false);
        ui->actionLineas->setCheckable(false);

    //Para dibujar Circulos
    mDefine=4;
}


