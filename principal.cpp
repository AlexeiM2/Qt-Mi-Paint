#include "principal.h"
#include "ui_principal.h"
#include <qgraphicsscene.h>
#include <QGraphicsView>
#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::black);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::blue;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    // Levanta la bandera (para que se pueda dibujar)
    mPuedeDibujar = true;
    // Captura la posición (punto x,y) del mouse
    mInicial = event->pos();
    // Acepta el evento
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        // Acepta el evento
        event->accept();
        // Salir del método
        return;
    }
    // Capturar el punto a donde se mueve el mouse
    if(m_opc == 1)
        // Capturar el punto a donde se mueve el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
    // Actualizar la interfaz (repinta con paintEvent)
    update();
    // actualizar el punto inicial
    mInicial = mFinal;
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_opc == 2){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal= event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar lineas
        mPainter->setPen(pincel);
        mPainter->drawLine(mInicial,mFinal);
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }
    //Rectangulo
    if(m_opc == 3){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar rectangulos
        QRect rectangulo (mInicial, mFinal);
        mPainter->setPen(pincel);
        mPainter->drawRect(rectangulo);
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }
    //Circunferencia
    if(m_opc ==4){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar circunferencias
        QRectF circulos (mInicial, mFinal);
        mPainter->setPen(pincel);
        mPainter->drawEllipse(circulos);
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }

}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 20);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::black);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    guardar();
}

void Principal::on_btnNuevo_clicked()
{
    mImagen->fill(Qt::black);
    mNumLineas = 0;
    update();
}


void Principal::on_btnGuardar_clicked()
{
    guardar();
}

void Principal::guardar()
{
    // Abrir cuadro de diálogo para obtener el nombre del archivo
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes .png (*.png)");
    // Validar que el nombre del archivo no sea vacío
    if ( !nombreArchivo.isEmpty() ){
        // Guardar imagen
        if (mImagen->save(nombreArchivo)){
            // Si todo va bien, muestra un mensaje de información
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        } else{
            // Si hay algún error, muestro advertencia
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
        }
    }
}

void Principal::abrir()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("AbrirImagen"), "/path/to/default/directory", tr("Image Files (*.png)"));
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    mImagen= new QImage(fileName);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->begin(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    //mPainter->begin(mImagen);
    mPainter->setPen(pincel);
    mPainter->drawImage(QPoint(0, 0), *mImagen);
    //mPainter->end(); //ya se puede dibujar encima porque??
    //entiendo que al terminar deberia volver a comenzar a dibujar
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene();
    view->setScene(scene);
    scene->addPixmap(QPixmap::fromImage(*mImagen));
    this->show();
}


void Principal::on_btnAbrir_clicked()
{
    abrir();
}


void Principal::on_actionAbrir_triggered()
{
    abrir();
}


void Principal::on_btnLibre_clicked()
{
    m_opc = 1;

}

void Principal::on_btnLineas_clicked()
{
    m_opc = 2;
    m_toogle = true;
}


void Principal::on_btnRec_clicked()
{
     m_opc = 3;
}

void Principal::on_btnCir_clicked()
{
    m_opc = 4;
}

