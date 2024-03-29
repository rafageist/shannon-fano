# Implementation of Shannon-Fano encoding algorithm

In the field of data compression, Shannon–Fano coding, named after Claude Shannon 
and Robert Fano, is a name given to two different but related techniques for constructing 
a prefix code based on a set of symbols and their probabilities (estimated or measured).

Shannon's method chooses a prefix code where a source symbol i {\displaystyle i} i is given 
the codeword length l i = ⌈ − log 2 ⁡ p i ⌉ {\displaystyle l_{i}=\lceil -\log _{2}p_{i}\rceil } 
{\displaystyle l_{i}=\lceil -\log _{2}p_{i}\rceil }. 

One common way of choosing the codewords uses the binary expansion of the cumulative probabilities. 
This method was proposed in Shannon's "A Mathematical Theory of Communication" (1948), his article 
introducing the field of information theory.

Fano's method divides the source symbols into two sets ("0" and "1") with probabilities 
as close to 1/2 as possible. Then those sets are themselves divided in two, and so on, 
until each set contains only one symbol. The codeword for that symbol is the string of "0"s 
and "1"s that records which half of the divides it fell on. This method was proposed in a 
later technical report by Fano (1949).

Shannon–Fano codes are suboptimal in the sense that they do not always achieve the lowest 
possible expected codeword length, as Huffman coding does.[1] However, Shannon–Fano codes 
have an expected codeword length within 1 bit of optimal. Fano's method usually produces 
encoding with shorter expected lengths than Shannon's method. However, Shannon's method is 
easier to analyse theoretically.

Shannon–Fano coding should not be confused with Shannon–Fano–Elias coding (also known as Elias coding), 
the precursor to arithmetic coding. 
