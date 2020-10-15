# Vladahashas
This is a university project with the goal of creating a hashing function that attempts to meet some of the standards of modern hashing functions.

## Hash function idea
![Hash function pseudocode](images/pseudocode.png)<br/>
Here you can see some pseudocode that demonstrates how the hashing function works.<br/>
1. Firstly, the hashing function itself takes an input of a string and then chops it up into large chunks (referred to as bitblocks) store in a ```std::deque<std::bitset<>>``` container.<br/>
2. The bitblocks are divided into smaller and smaller partitions whose bits are either reversed or inverted.<br/>
3. A similar process occurs, but with a slightly different approach: it iterates through each bit and reverses bits on one side of it, and inverts them on the other depending on whether the bit is 1 or 0.<br/>
4. This process is repeated for (currently) 4 times to achieve a satisfactory avalanche effect.

## Experimentational analysis
Analysis was done to test and confirm the following standards for the hashing function:
1. The input can be of any length
2. The output must be of fixed size (256 bits)
3. The function must be deterministic
4. The function should be fairly efficient (linear time)
5. Two hashes from relatively similar strings should be vastly different
6. Collisions should be unfeasible to find (finding a string that matches the hash of another string)

The following files were used for testing:<br/>
test1_1.txt - contains the letter 'a'<br/>
test1_2.txt - contains the letter 'b'<br/>
test2_1.txt, test2_2, test3_1 - 1001 random characters<br/>
test3_2.txt - same character as in test3_1, except a single character is changed<br/>
test4.txt - empty file<br/>
### Input of any length
![Console output](images/anyinput.png)<br/>
Confirmed, input can be of any length.
### Output of fixed size
![Console output](images/fixedoutput.png)<br/>
Confirmed, output is of fixed size (256 bits).
### Hashing function is deterministic
![Console output](images/outputdeterministic.png)<br/>
Confirmed, output stays the same for the same input.
### Hashing function is efficient
![Console output](images/hashefficiency.png)<br/>
The design of the algorithm limits the hashing function to O(n^2) complexity, thus the function is not efficient.
### Two hashes from similar strings should differ vastly
![Console output](images/outputvastlydifferent.png)<br/>
Confirmed, the outputs differ vastly even thought the input is similar.
### Collisions unfeasible to find
This has yet to be tested.

## Conclusions
The hashing algorithm meets most of the requirements except time efficiency, it is not efficient at all. Also, it has not yet been tested for collisions, so that part is unknown as well.
