/* TODO
 * ----------
 * Proposer une méthode enableYRightAxis(int ratio)
 *  ration : ratio par rapport à l'axe y de gauche
 *  C'est seulement dans cette méthode que je connect le signal
 *  zoomed de l'axe y de gauche et que j'applique un zoom avec le
 *  zoomer de l'ace de droite en rescpectant le ration
 *
 *  Proposer la meme chose pour l'axe des X ???? p-e :p
 *
 *  Proposer de modifier ce rapport dans le menu :o ?
 *      dans ce cas, une fois le rapport changer, faire un nouveau zoom
 *      sur l'ensemble des courbes pour que les echelles s'adaptent
 *
 *  Proposer que le label du curseur affiche la position en y de l'un ou
 *  l'autre axe y --> pour ça, gros changements:
 *      - desactiver le rubberBandRect et l'activer du bon coté
 *      - connecter aussi bien le signal zoomed dans deux zoomers
 *        et dans un des slot on feras un / par le rapport et dans l'autre
 *        slot on fera un * le rapport
 *  Attention aussi que si on ne peux proposer (dans le menu edit -> graphics)
 *  de changer le label qui si les deux axes sont affichés !!!!
 */


#ifndef __PLOT_HPP__
#define __PLOT_HPP__

// QT includes
#include <QMenu>

// Qwt includes
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_legend_item.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_magnifier.h>
#include "Zoomer.hpp"

class Plot : public QwtPlot
{
    Q_OBJECT

    public:

        explicit Plot(QString const& title, QWidget* parent = NULL);
        explicit Plot(QwtText const& title, QWidget* parent = NULL);
        virtual ~Plot(void);

        // Getter
        bool isGridVisible(void) const;
        bool isCrossLineVisible(void) const;
        bool isLabelPositionVisible(void) const;

    public slots:

        void setGridVisible(bool visible);
        void setCrossLineVisible(bool visible);
        void setLabelPositionVisible(bool visible);

    protected slots:

        // Personnal slots
        void updateCrossLinePosition(QPointF const& pos);
        void showLegendContextMenu(QPoint const& pos);

        // Slot de test
        void adaptYRightAxis(QRectF const& rect);

    protected:

        QwtLegend*        legend;
        QMenu*            legendContextMenu;
        QwtPlotGrid*      grid;
        QwtPlotMarker*    crossLine;
        QwtPlotPanner*    panner;
        Zoomer*           yRightZoomer;
        Zoomer*           yLeftZoomer;
        QwtPlotMagnifier* magnifier;
};

#endif /* __PLOT_HPP__ */
