# Documentation of my exploration into Elliptic Curve Cryptography
## Code based off the brilliant book on the subject - "Implementing Elliptic Curve Cryptography"

None of that wishy washy RSA stuff. This is the real deal.

Ceasars shift? Psh. More like Ceasar's Salad.

Should be fun. Keep an eye on this page for updates.


## Update 1: 25th May 2017 - BigInt Implementation
Just the initial boilerplate to represent the large numbers used in this kind of cryptography.
Essentially, we're storing the values in chunks equal to the word size on the intended system.
(Why word size? - the word size is usually the natural base size for processor operations, so storing
values in terms of that size will optimize the efficiency). 


## Update 2: 26th May 2017 - BigInt functions
Added some more stuff from the book. Most of the code is unchanged, however have added my own comments.
Wrapping my head around this stuff certainly took a while - for the purpose of this book, I'll continue using
this BIGINT implementation, however, afterwards I want to implement my own arbitrary size version of the implementation.
Also have a look into how to optimize this kind of coding construct. We'll keep it simple for now to aid the digestion
of the more complex subject at hand, Elliptic curve cryptography.

## Update 3: 27th May 2017 - GCD Functionality
More code from the book, mainly implementing the extended GCD algorithm in terms of the BIGINT number system introduced earlier
into the book. While I have covered the GCD division algorithm in a First-Year CS Maths course, it was the first time I had met the 
computerized version. 

## Update 4: 28th May 2017 - Modular Inverses and Powers
Implemented more code from the book. This time added modular inverse and power functions. For the modular power function, I was able
to use loop invariants (Hooare Logic) to provide some extra proof behind it's correctness. At the momeent I don't fully understand the modular inverse code - I need to work through some examples on paper before I can gain a better understanding of the method behind that. Should be able
to add in the extra explanatory comments soon.

## Update 5: 29th May 2017 - Polynomial Base Maths
Continued progressing through the book, I spent some tiem today considering how to interpret some of the GCD and Modular Inverse functions - sure I've bene through the steps and the "human" method of finding these values, but the concept behind the computer implementation continues to elude me. The book I'm reading - look at the title, has a reference to some guides on the maths behind the implementation. Also polynomial maths is sweet, and even sweeter is using a modular base. I have a rough idea of why the fact that polynomial coefficients are linear maps allows us to consider modulo (as they are linear maps (f (k)) x^3  + (f(j)) x*3 == (f(k) + f(j)) x^3 and if f is distributive over addition == f(k+j). Thus when considering modulos, (k mod b) x^ p + (j mod b) x^ p == ((k+j) mod b) x^p thus allowing modular polynomial fields. But I still think I need some more paper tests to fully comprehend the subject.

## Update 6: 31st May 2017 - More polynomial Maths
Was mainly busy the SheCodes Morgan Stanley Hackathon, however managed to fit in some time to continue work on the algorithm. Unfortunately was a few minutes late to upload the code before midnight so I ended up having no contributions for this day. The polynomial maths is quite enjoyable - often many of the algorithms seem very complex and intimidating, however as you parse through them, you get a real sense of accomplishment as you decipher the intention of each implementation detail.

## Update 7: 2nd June 2017 - Even More polynomial Maths
Was recently busy going to London to organize work experience over the summer, and as such didn't have the oppourtunity to continue work on this project. Spent most of my allocated time on this project getting back to scratch on how the project works again. While some of the intricacies of how the bit fields are still a little confusing to me, I plan to continue through the book until I can understand the high level intuition behind eliptic curve cryptography. At which point I can then go back and rewrite the underlying C structures in a way that makes more sense to me.

## Update 8: 3rd June 2017 - Finished Polynomial utility functions
Spent most of the day learning android, thus had less time to work on the Polynomial library. I implemented some more functions from the book. I'm slightly annoyed at the fact that despite investing an hour or so into deciphering the polynomial maths, I have made little in the way of headway. At this point, I think I will have to invest my time into researching some other sources for gaining the correct intuition.
