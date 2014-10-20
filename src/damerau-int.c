/* ugexe@cpan.org (Nick Logan)    */

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Our unsorted dictionary linked list.   */
/* Note we use character ints, not chars. */

struct dictionary{
    unsigned int key;
    unsigned int value;
    struct dictionary* next;
};
typedef struct dictionary item;


static __inline item* push(unsigned int key,item* curr){
    item* head;
    head = malloc(sizeof(item));   
    head->key = key;
    head->value = 0;
    head->next = curr;
    return head;
}


static __inline item* find(item* head,unsigned int key){
    item* iterator = head;
    while(iterator){
        if(iterator->key == key){
          return iterator;
        }

        iterator = iterator->next;
    }
   
    return NULL;
}

static __inline item* uniquePush(item* head,unsigned int key){
    item* iterator = head;
    while(iterator){
        if(iterator->key == key){
          return head;
        }

        iterator = iterator->next;
    }
   
    return push(key,head); 
}

static void dict_free(item* head){
    item* iterator = head;
    while(iterator){
        item* temp = iterator;
        iterator = iterator->next;
        free(temp);
    }

    head = NULL;
}

/* End of Dictionary Stuff */



 
/* All calculations/work are done here */
static int distance(unsigned int src[],unsigned int tgt[],unsigned int x,unsigned int y,unsigned int maxDistance){
    item *head = NULL;
    unsigned int swapCount,swapScore,targetCharCount,i,j;
    unsigned int *scores = malloc( (x + 2) * (y + 2) * sizeof(unsigned int) );
    unsigned int score_ceil = x + y;
    unsigned int diff = MAX(x , y) - MIN(x, y);
    unsigned int mdx = (maxDistance == 0) ? MAX(x,y) : maxDistance;

    /* intialize matrix start values */
    scores[0] = score_ceil;  
    scores[1 * (y + 2) + 0] = score_ceil;
    scores[0 * (y + 2) + 1] = score_ceil;
    scores[1 * (y + 2) + 1] = 0;
    head = uniquePush(uniquePush(head,src[0]),tgt[0]);

    /* work loops    */
    /* i = src index */
    /* j = tgt index */
    for(i=1;i<=x;i++){ 
        head = uniquePush(head,src[i]);
        scores[(i+1) * (y + 2) + 1] = i;
        scores[(i+1) * (y + 2) + 0] = score_ceil;
        swapCount = 0;

        for(j=1;j<=y;j++){
            if(i == 1) {
                head = uniquePush(head,tgt[j]);
                scores[1 * (y + 2) + (j + 1)] = j;
                scores[0 * (y + 2) + (j + 1)] = score_ceil;
            }

            targetCharCount = find(head,tgt[j-1])->value;
            swapScore = scores[targetCharCount * (y + 2) + swapCount] + i - targetCharCount - 1 + j - swapCount;

            if(src[i-1] != tgt[j-1]){      
                scores[(i+1) * (y + 2) + (j + 1)] = MIN(swapScore,(MIN(scores[i * (y + 2) + j], MIN(scores[(i+1) * (y + 2) + j], scores[i * (y + 2) + (j + 1)])) + 1));
            }
            else{ 
                swapCount = j;
                scores[(i+1) * (y + 2) + (j + 1)] = MIN(scores[i * (y + 2) + j], swapScore);
            }

            /*
            warn("i:%d j:%d mdx:%d maxDistance:%d y:%d",i,j,mdx,maxDistance,y);
            if( i == j ) {
                if(mdx < scores[(i+1) * (y + 2) + (j + 1)]) {
                    dict_free(head);
                    free(scores);
                    return -20;
                }
            }
            else if( j >= x && j > i && mdx < (scores[(i+1) * (y + 2) + (j + 1)] + (MAX(diff,j) - MIN(diff,j) - 1)) ) {
                // we can look at the length difference along with the current distance to determine a minimum distance
                return -30;
            }
            */

        }

        find(head,src[i-1])->value = i;
    }

    {
        unsigned int score = scores[(x+1) * (y + 2) + (y + 1)];
        dict_free(head);
        free(scores);
        return (maxDistance != 0 && maxDistance < score)?(-1):score;
    }
}