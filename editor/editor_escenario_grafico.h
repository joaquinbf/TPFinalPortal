#ifndef EDITOR_ESCENARIO_GRAFICO_H
#define EDITOR_ESCENARIO_GRAFICO_H

#include <QGraphicsScene>
#include <QSpinBox>
#include <string>
#include <QVector>
//#include "yaml-cpp/yaml.h"
#include "editor_item_grafico.h"
#include "editor_celda_grafica.h"

class EscenarioGrafico : public QGraphicsScene
{
private:
    QSpinBox *spinBoxX;
    QSpinBox *spinBoxY;

    std::string fondoEscenario;

    QVector<CeldaGrafica> celdas;

    unsigned idClassACrear;

public:
    EscenarioGrafico();
    virtual ~EscenarioGrafico();
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event);

    void setSpinBox(QSpinBox *spinBoxX_, QSpinBox *spinBoxY_);
    void setFondoEscenario(std::string direccion);
    void setIdClassACrear(unsigned idClass);

    void agregarACeldas(ItemGrafico *item, QGraphicsSceneMouseEvent *event);
    void moverItem(QGraphicsSceneMouseEvent *event);
    void crearItem(QGraphicsSceneMouseEvent *event);
    void crearPersonaje(QGraphicsSceneMouseEvent *event);
    void crearBloqueDeRoca(QGraphicsSceneMouseEvent *event);



    CeldaGrafica &getCelda(QPointF posicion);

    void guardar(/*YAML::Node &nodo*/);

};

#endif // EDITOR_ESCENARIO_GRAFICO_H
