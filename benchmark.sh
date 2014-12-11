for i in {1..4}
do
    echo "Using $i processors"
    for j in {1..3}
    do
        gtimeout 1000 src/lda -est -alpha 0.01 -beta 0.01 -ntopics 100 -niters 1050 -twords 20 -threads $i -fname yelp-$i-$j -dfile ~/Documents/files/finaltestfiles/headsampleyelp.txt >> yelpmhwldagossip
    done
done
