/*
 * Copyright (C) 2007 by
 * 
 * 	Xuan-Hieu Phan
 *	hieuxuan@ecei.tohoku.ac.jp or pxhieu@gmail.com
 * 	Graduate School of Information Sciences
 * 	Tohoku University
 *
 * GibbsLDA++ is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * GibbsLDA++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GibbsLDA++; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

/* 
 * References:
 * + The Java code of Gregor Heinrich (gregor@arbylon.net)
 *   http://www.arbylon.net/projects/LdaGibbsSampler.java
 * + "Parameter estimation for text analysis" by Gregor Heinrich
 *   http://www.arbylon.net/publications/text-est.pdf
 */

#ifndef	_WALKER_H
#define	_WALKER_H

#include <vector>

using namespace std;


class model;

struct topicprob {
    int topic;
    double prob;
};

struct bucket {
    int lowtopic;
    int hightopic;
    double problow;
};

// Walker alias
class walker {
public:
    int w;                  // the word id
    int K;                  // the number of topics
    double Q;               // normalizer
    vector<bucket> buckets; // buckets for deciding topics    
    double * topicprobs;    // unnormalized probabilities of topics
    vector<int> samples;    // samples (if any)

    walker (int w, model * mod, int * localnwsum);
    ~walker();

    // set default values for variables
    void pull_samples();
    int next_topic();
    bool is_empty();
};

#endif

