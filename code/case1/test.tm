; This example program checks if the length of the input 0-string is a Fibonacci number.
; Input: a string of 0's, eg '000000000'

; the finite set of states
#Q = {0,pre,cmp01,mh0_1,mh1_1,mh1_2,mh2_1,mh2_2,mh3_1,mh3_2,cp12,cp23,cpadd31,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {0}

; the complete set of tape symbols
#G = {0,_,T,r,u,e,F,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 4

; the transition functions

; State 0: begin
0 **** **** **** pre
0 ____ ____ **** reject_mh0     ; Empty input

; State pre: initial
pre *___ *00_ **** cmp01

; State cmp01: compare two strings(tape0 & tape 1)
cmp01 00** 00** rr** cmp01
cmp01 __** __** ll** accept_mh0
cmp01 0_** 0_** *l** mh3_1
cmp01 _0** _0** l*** reject_mh0

; State mh3_1: move 3st head to the left
mh3_1 ***_ ***_ ***r mh2_1
mh3_1 **** **** ***l mh3_1

; State mh2_1: move 2st head to the left
mh2_1 **_* **_* **r* cp23
mh2_1 **** **** **l* mh2_1

; State cp23: copy the string of tape2 to tape3
cp23 **0* **00 **rr cp23
cp23 **__ **__ **ll mh2_2

; State mh2_2: move 2st head to the left
mh2_2 **_* **_* **r* mh1_1
mh2_2 **** **** **l* mh2_2

; State mh1_1: move 1st head to the left
mh1_1 *_** *_** *r** cp12
mh1_1 **** **** *l** mh1_1

; State cp12: copy the string of tape1 to tape2
cp12 *0** *00* *rr* cp12
cp12 *__* *__* **l* mh3_2

; State mh3_2: move 3st head to the left
mh3_2 ***_ ***_ ***r cpadd31
mh3_2 **** **** ***l mh3_2

; State cpadd31: copy the string of tape3 and add to tape1
cpadd31 ***0 *0*0 *r*r cpadd31
cpadd31 *_*_ *_*_ *l*l mh1_2

; State mh1_2: move 1st head to the left
mh1_2 *_** *_** *r** mh0_1
mh1_2 **** **** *l** mh1_2

; State mh0_1: move 0st head to the left
mh0_1 _*** _*** r*** cmp01
mh0_1 **** **** l*** mh0_1

; State accept_mh0: move 0st head to the left
accept_mh0 _*** _*** r*** accept
accept_mh0 **** _*** l*** accept_mh0

; State reject_mh0: move 0st head to the left
reject_mh0 _*** _*** r*** reject
reject_mh0 **** _*** l*** reject_mh0

accept _*** T*** r*** accept2
accept2 _*** r*** r*** accept3
accept3 _*** u*** r*** accept4
accept4 _*** e*** **** halt_accept

reject _*** F*** r*** reject2
reject2 _*** a*** r*** reject3
reject3 _*** l*** r*** reject4
reject4 _*** s*** r*** reject5
reject5 _*** e*** **** halt_reject