/*
 *   libpal - Automated Placement of Labels Library     http://pal.heig-vd.ch
 *
 *
 *   Copyright (C) 2007, 2008 MIS-TIC, HEIG-VD (University of Applied Sciences Western Switzerland)
 *   Copyright (C) 2008, 2009 IICT-SYSIN, HEIG-VD (University of Applied Sciences Western Switzerland)
 *
 *
 * This file is part of libpal.
 *
 * libpal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libpal. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef _PROBLEM_H
#define _PROBLEM_H

#include <list>
#include "pal.h"
#include "rtree.hpp"

namespace pal {

    class LabelPosition;
    class Label;

    class Sol {
    public:
        int *s;
        double cost;
    };

    typedef struct _subpart {
        /**
         * # of features in problem
         */
        int probSize;

        /**
         * # of features bounding the problem
         */
        int borderSize;

        /**
         *  total # features (prob + border)
         */
        int subSize;

        /**
         * wrap bw sub feat and main feat
         */
        int *sub;
        /**
         * sub solution
         */
        int *sol;
        /**
         * first feat in sub part
         */
        int seed;
    } SubPart;

    typedef struct _chain {
        int degree;
        double delta;
        int *feat;
        int *label;
    } Chain;

    /**
     * \brief Represent a problem
     */
    class Problem {

        friend class Pal;

    private:

        /**
         * How many layers are lebelled ?
         */
        int nbLabelledLayers;

        /**
         * Names of the labelled layers
         */
        char **labelledLayersName;

        /**
         * # active candidates (remaining after reduce())
         */
        int nblp;
        /**
         * # candidates (all, including)
         */
        int all_nblp;

        /**
         * # feature to label
         */
        int nbft;


        /**
         * if true, special value -1 is prohibited
         */
        bool displayAll;

        /**
         * Map extent (xmin, ymin, xmax, ymax)
         */
        double bbox[4];

        /**
         * map scale is 1:scale
         */
        double scale;

        double *labelPositionCost;
        int *nbOlap;

        LabelPosition **labelpositions;

        RTree<LabelPosition*, double, 2, double> *candidates;  // index all candidates
        RTree<LabelPosition*, double, 2, double> *candidates_sol; // index active candidates
        RTree<LabelPosition*, double, 2, double> *candidates_subsol; // idem for subparts

        //int *feat;        // [nblp]
        int *featStartId; // [nbft]
        int *featNbLp;    // [nbft]
        double *inactiveCost; //

        Sol *sol;         // [nbft]
        int nbActive;

        int nbOverlap;

        int *featWrap;

        Chain *chain (SubPart *part, int seed);

        Chain *chain (int seed);

        Pal *pal;

        void solution_cost();
        void check_solution();

        Problem();

        //Problem(char *lorena_file, bool displayAll);

        ~Problem();


        void reduce();


        void post_optimization();



        /**
         * \brief popmusic framework
         */
        void popmusic();

        /**
         * \brief Test with very-large scale neighborhood
         */
        void chain_search();

        std::list<Label*> * getSolution (bool returnInactive);

        PalStat * getStats();

        /* usefull only for postscript post-conversion*/
        //void toFile(char *label_file);

        SubPart *subPart (int r, int featseed, int *isIn);

        void initialization();

        double compute_feature_cost (SubPart *part, int feat_id, int label_id, int *nbOverlap);
        double compute_subsolution_cost (SubPart *part, int *s, int * nbOverlap);

        double popmusic_chain (SubPart *part);

        double popmusic_tabu (SubPart *part);
        double popmusic_tabu_chain (SubPart *part);

        void init_sol_empty();
        void init_sol_falp();

#ifdef _EXPORT_MAP_
        void drawLabels (std::ofstream &svgmap);
#endif

    };

} // namespace

#endif
