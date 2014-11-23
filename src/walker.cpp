
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

#include <random>
#include <stdlib.h>
#include "walker.h"

using namespace std;

walker::walker(int w, model mod) {
    this.w = w;
    this.K = mod.K;
    H = queue<topicprob>();
    L = queue<topicprob>();
    double totalprob = 0.0
    double threshold = 1.0 / K;
    for (i = 0; i < K; ++i) {
        double probprop = model.alpha * (model.nw[w][i] + model.beta) / (model.nwsum[i] + (model.beta * model.V));
        if (probprop > threshold) {
            H.push(topicprob(i, probprop));
        } else {
            L.push(topicprob(i, probprop));
        }
        totalprob += probprop;
    }

    buckets = vector<bucket>();    
    while (L.size() > 0 && H.size() > 0) {
        topicprob low = L.pop();
        if (low.prob / totalprob == threshold) {
            newbucket = bucket(low.topic, low.topic, 1.0);
            continue;
        }
        topicprob high = H.pop();
        newbucket = bucket(low.topic, high.topic, low.prob * K / totalprob);
        buckets.push_back(newbucket);

        double probleft = high.prob - threshold + low.prob;
        if (probleft > threshold) {
            H.push(topicprob(high.topic, probleft));
        } else {
            L.push(topicprob(high.topic, probleft));
        }
    }

    while (L.size() > 0) {
        topicprob low = L.pop();
        if (low.prob == threshold) {
            newbucket = bucket(low.topic, low.topic, 1.0);
            continue;
        }
        topicprob high = L.pop();
        newbucket = bucket(low.topic, high.topic, low.prob * K / totalprob);
        buckets.push_back(newbucket);
        }
    }
}

walker::~walker() {
    if (topicprobs) {
        delete [] topicprobs;
    }
}


void walker::set_default_values() {
    w = 0;
    K = 0;
    H = queue<topicprob>();
    L = queue<topicprob>();
    buckets = vector<bucket>();
}

int * walker::pull_samples() {
    default_random_engine gen();
    uniform_real_distribution<double> dist(0.0, 1.0);
    int * samples = int[K];

    for (i = 0; i < K; ++i) {
        int buck = rand() % K;
        double sample = dist(gen);
        if (sample < buckets[i].problow) {
            samples[i] = buckets[i].lowtopic;
        } else {
            samples[i] = buckets[i].hightopic;
        }
    }

    return samples;
}
