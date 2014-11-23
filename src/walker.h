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

#include <queue.h>
#include "model.h"

using namespace std;

struct topicprob {
    int topic;
    double prob;
}

struct bucket {
    int lowtopic;
    int hightopic;
    double problow;
}

// Walker alias
class walker {
public:
    int w;                  // the word id
    int K;                  // the number of topics
    double * topicprobs;    // topicprob[k]: the multinomial probability of k
    queue<topicprob> H;     // topics with greater than 1/K probability
    queue<topicprob> L;     // topics with less than or equal to 1/L probability
    vector<bucket> buckets; // buckets for deciding topics    


    walker() {
        set_default_values();
    }

    walker (int w, model mod);
    ~walker();
    
    // set default values for variables
    int * pull_samples();
};

#endif

