from collections import Counter
import numpy as np

R = 20


# Find alpha, beta, ntopics, ndocs, nwords
otherf = open('yelp-1-1-final.others', 'r')
constvars = {}
for line in otherf:
    var, val = line.split('=')
    constvars[var] = val
otherf.close()
alpha = float(constvars['alpha'])

# Get a word to id mapping
worddict = {}
wordsf = open('yelp.words', 'r')
for line in wordsf:
    num, word = line.split()
    worddict[word] = int(num)

wordsf.close()

# Initialize phi
phif = open('yelp-1-1-final.phi', 'r')
phi = {}

for topic, line in enumerate(phif):
    phi[topic] = map(float, line.split())

phif.close()

docs = {}
# Read new file of documents
testf = open('testreviews.txt', 'r')
for docid, line in enumerate(testf):
    words = line.split()
    idlist = []
    for word in words:
        if word in worddict:
            idlist += [worddict[word]]

    docs[docid] = idlist


# Compute likelihood
likelihood = 0.0
ntopics = len(phi)
words_so_far = 0

for docid, docidlist in enumerate(docs.itervalues()):
    words_so_far += len(docidlist)
    # Our lists of generated samples
    zs = [[-1 for i in docidlist] for rr in xrange(R)]
    
    # Our counts of topics so far for each of the chains of samples
    n = [Counter() for rr in xrange(R)]

    # For each word in the document, left-to-right
    for index, wid in enumerate(docidlist):
        p_n = 0.0
        # For each particle
        for r in xrange(R):
            # For each index before our current one
            for n_prime in xrange(index):
                # Decrement the count for the old topic (-1 if unassigned)
                n[r][zs[r][n_prime]] -= 1
                unnormed_pks = [phi[k][docidlist[n_prime]] * (n[r][k] + alpha) for k in xrange(ntopics)]
                norm_const = sum(unnormed_pks)
                normed_pks = [pk/norm_const for pk in unnormed_pks]
                zs[r][n_prime] = np.random.multinomial(1, normed_pks)[0]
                n[r][zs[r][n_prime]] += 1
            # Add the likelihood estimate for that token to our token prob
            p_n += sum([phi[k][wid] * (n[r][k] + alpha) / (index + (ntopics * alpha))
                       for k
                       in xrange(ntopics)])
            
            # Sample our current token
            unnormed_pks = [phi[k][wid] * (n[r][k] + alpha) for k in xrange(ntopics)]
            norm_const = sum(unnormed_pks)
            normed_pks = [pk/norm_const for pk in unnormed_pks]
            zs[r][index] = np.random.multinomial(1, normed_pks)[0]
            n[r][zs[r][index]] += 1

        p_n = p_n / R
        likelihood += np.log(p_n)
    print "Document {0}: accumulated probability {1}, per word {2}".format(docid, likelihood, likelihood / words_so_far)
print "Total likelihood", likelihood
print "Likelihood per word", likelihood / int(constvars["nwords"])
                

