; This example program checks if the input string is a ww structure.
; Input: a string of 0's and 1's, eg '1001001'

; the finite set of states
#Q = {0,mv_right,judge,mh2_1,cp02,mh2_2,mh1_1,cmp12,mh1_2,mh0_1,mh2_3,mvr,accept_mh0,reject_mh0,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_,T,r,u,e,F,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

; State 0: begin
0 *** *** *** mv_right
0 ___ ___ *** reject_mh0     ; Empty input

; State mv_right: move right for one step
mv_right 0** 00* r** judge
mv_right 1** 11* r** judge

; State judge: judge weather it is empty
judge _** *** l** reject_mh0
judge *** *** *** mh2_1

; State mh2_1: move 2st head to the left
mh2_1 **_ **_ **r cp02
mh2_1 *** **_ **l mh2_1

; State cp02: copy the string of tape0 to tape2
cp02 0** 0*0 r*r cp02
cp02 1** 1*1 r*r cp02
cp02 _*_ _*_ l*l mh2_2

; State mh2_2: move 2st head to the left
mh2_2 **_ **_ **r mh1_1
mh2_2 *** *** **l mh2_2

; State mh1_1: move 1st head to the left
mh1_1 *_* *_* *r* cmp12
mh1_1 *** *** *l* mh1_1

; State cmp12: compare two strings(tape1 & tape 2)
cmp12 *00 *00 *rr cmp12
cmp12 *11 *11 *rr cmp12
cmp12 *__ *__ *ll accept_mh0
cmp12 *** *** *** mh1_2
cmp12 *_* *_* *l* mh1_2

; State mh1_2: move 1st head to the left
mh1_2 *_* *_* *r* mh0_1
mh1_2 *** *** *l* mh1_2

; State mh0_1: move 0st head to the left
mh0_1 _** _** r** mh2_3 
mh0_1 *** *** l** mh0_1

; State mh2_3: move 2st head to the right
mh2_3 **_ **_ **l mvr
mh2_3 *** *** **r mh2_3

; State mvr: move tape0 & tape1 right
mvr *_* *_* *** mv_right
mvr *** *** rr* mvr

; State accept_mh0: move 0st head to the left
accept_mh0 _** _** r** accept
accept_mh0 *** _** l** accept_mh0

; State reject_mh0: move 0st head to the left
reject_mh0 _** _** r** reject
reject_mh0 *** _** l** reject_mh0

accept _** T** r** accept2
accept2 _** r** r** accept3
accept3 _** u** r** accept4
accept4 _** e** *** halt_accept

reject _** F** r** reject2
reject2 _** a** r** reject3
reject3 _** l** r** reject4
reject4 _** s** r** reject5
reject5 _** e** *** halt_reject