for i in {1..4}
do
    echo "Using $i processors"
    for j in {1..5}
    do
        time src/lda -est -alpha 0.01 -beta 0.05 -ntopics 100 -niters 1000 -twords 20 -threads $i -fname yelp-$i-$j -dfile ~/research/20kreviews.txt >> yelpmhwldashort
    done
done
