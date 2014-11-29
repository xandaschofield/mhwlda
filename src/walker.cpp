
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

#include <queue>
#include <random>
#include <stdlib.h>
#include "walker.h"
#include "model.h"

using namespace std;

walker::~walker()
{
    delete [] topicprobs;
}

walker::walker(int w, model * mod) : w(w), K(mod->K), Q(0.0) 
{
    queue<topicprob> H;
    queue<topicprob> L;
    topicprobs = new double[K];
    double totalprob = 0.0;
    double threshold = 1.0 / K;
    for (int i = 0; i < K; ++i) {
        double probprop = mod->alpha * (mod->nw[w][i] + mod->beta) / (mod->nwsum[i] + (mod->beta * mod->V));
        topicprobs[i] = probprop;
        Q += probprop;
    }
    for (int k = 0; k < K; ++k) {
        double normprob = topicprobs[k] / Q;
        if (normprob > threshold) {
            H.push((topicprob){.topic = k, .prob = normprob});
        } else {
            L.push((topicprob){.topic = k, .prob = normprob});
        }
    }

    buckets = vector<bucket>();    
    while (L.size() > 0 && H.size() > 0) {
        topicprob low = L.front();
        if (low.prob == threshold) {
            bucket newbucket = (bucket){low.topic, low.topic, 1.0};
            buckets.push_back(newbucket);
            L.pop();
            continue;
        }
        topicprob high = H.front();
        bucket newbucket = (bucket){low.topic, high.topic, low.prob * K};
        buckets.push_back(newbucket);
        H.pop();

        double probleft = high.prob - threshold + low.prob;
        if (probleft > threshold) {
            H.push((topicprob){high.topic, probleft});
        } else {
            L.push((topicprob){high.topic, probleft});
        }
    }

    while (L.size() > 0) {
        topicprob low = L.front();
        bucket newbucket = (bucket){low.topic, low.topic, 1.0};
        buckets.push_back(newbucket);
        L.pop();
    }

    while (H.size() > 0) {
        topicprob high = H.front();
        bucket newbucket = (bucket){high.topic, high.topic, 1.0};
        buckets.push_back(newbucket);
        H.pop();
    }
}

void walker::pull_samples() {
    for (int i = 0; i < K; ++i) {
        int buck = rand() % K;
        double sample = ((double)random() / RAND_MAX);
        if (sample < buckets[i].problow) {
            samples.push_back(buckets[i].lowtopic);
        } else {
            samples.push_back(buckets[i].hightopic);
        }
    }
}

int walker::next_topic() {
    int ret = samples.back();
    samples.pop_back();
    return ret;
}

bool walker::is_empty() {
    return (samples.size() == 0);
}