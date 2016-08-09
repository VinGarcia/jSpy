# jSpy - Javascript-like Speech Parser framework with Pythonic features.

This project provides an interpreter for the jSpy programming language.

This language is built aiming to join together some of the best features
from JavaScript and Python. So at the same time it tries to follow [The
Zen of Python][python-zen] and an easy use of generators it also offers
protypical inheritance, and function closures.

[python-zen]: https://www.python.org/dev/peps/pep-0020/

The jSpy language was created as a way to provide support to a new Speech
Parser technique developed by VinGarcia originally called `Talkin`. This
Parser is built-in into the language and can be accessed by using the
reserved word `matcher` (See the examples section).

*This project was created by Vinícius Garcia and relies heavily on
[Brandon Amos][brandon] [C++ Expression Parser][exp-parser] library.*

[brandon]: http://bamos.github.io
[exp-parser]: https://github.com/bamos/cpp-expression-parser

---

# Features.
 + User defined functions support
 + Support for list and map containers
 + Support for prototypical inheritance
 + Garbage Collection by Reference Counting (not good with cyclic references)
 + Support for the `Talkin` matcher expressions.

---

# Minimal examples

## Hello World:

```Javascript
make shell

JSpy 0.2a by Vinícius Garcia

print('hello world')
hello world
```

## Function usage:

```Javascript
a = 0
function a_inc() { a = a+1; return a }

print(a_inc())
1
print(a_inc())
2

function my_print(a,b,c) { print(a,b,c); print(arglist) }

my_print(1, 'two', None, 'and', 'some', 'more')
1 two None
[ "and", "some", "more" ]
```

## Data containers:

Map container:

```JavaScript
a = map()

a.attr1 = 10
a.attr2 = 20

print(a)
{ "attr1": 10, "attr2": 20 }

print(a.attr1, a['attr2'], a['attr3'])
10 20 None
```

List container:

```JavaScript
a = list()
a.push(1).push(2).push(3)

print(a)
[ 1, 2, 3 ]

print(a.len())
3

b = list('one', 2, "three")

print(b)
[ "one", 2, "three" ]

b_pop = b.pop()

print(b_pop)
"three"

print(b)
[ "one", 2 ]

// Pop the item at index 0:
b.pop(0)

print(b)
[ 2 ]
```

## Loops:

```JavaScript
for(n in range(5)) print(n)
0
1
2
3
4

for(n in range(10,4,-2)) print(n)
10
8
6

for(n in list(1,2,3)) print(n)
1
2
3

a = map()
a.a = 10
a.b = 20
a.c = 30

for(key in a) print(key)
a
b
c

// We also have while loops:
a = 3;
while(a > 0) { print(a); a = a-1; }
3
2
1
```

## Object Inheritance:

The jSpy object Inheritance was made so it would allow protypical inheritance
in a way it's not confusing and simillar enough to classical inheritance
so it's easy to learn:

```JavaScript
a = map()

a.name = 'A'

// Inheritance:
b = extend(a)

print(b)
{}

print(b.name)
A

b.name = 'B'

print(a.name, b.name)
A B

// Instantiation:
function b_init (attr) { this.attr = attr; }
b.__init__ = b_init;

// Make b the prototype of b1
// and execute __init__(10):
b1 = new(b, 10)

print(b1)
{ "attr": 10 }

print(b1.name)
B
```

Notes:

- There is not yet support for the `super` key-word.
- `new` and `extend` are functions and not key-words yet.

In future this should change so it looks prettier than it is now.

# Talkin Matchers

A Talkin Matcher works like an extended Regular Expression.
It was made to deal with [Natural language texts][NLP] and so it
is quite more complex and inefficient than a normal RegExp.

[NLP]: https://en.wikipedia.org/wiki/Natural_language_processing

The main features of this matcher are:

+ It does detect all matches possible, even when they are ambiguous
+ It can rely on other matchers to parse parts of its text
+ It can rely on boolean expressions to deny a match if wanted
+ It can associate a function with each expression for post-processing

Note that the function association allows recursive parsing of text.

It can then work as a very poweful irrestrict grammar.

## Examples

Simplest matcher possible:

```Javascript
matcher M1 "pattern" { print('matched!') };

M1.exec('pattern')
matched!

M1.exec('patterns')
// Nothing happens
```

A matcher works like an RegExp, so some functionalities
expected from a RegExp also work on Talkin expressions.

In future all `RegExp` commands should work, for now only some of them
are supported.

```Javascript
matcher M2 "[Pp]att*ern" { print(text) }

M2.exec('patttttttttern')
patttttttttern
```

## Complex examples

From now on the examples will be more complex.

They will not be fully explained here, but soon we should make a document
explaining all the syntax of the Talkin Matchers.

Example with multiple expressions in the same matcher:

```Javascript
matcher open {
  "open the library" if(there_is_library) { return 'library' }
  "open the door" if(there_is_door) { return 'door' }
}

print( open.exec('open the door') )
[ 'door' ]
```

Capturing a section of the text:

```Javascript
matcher Object {
  "door"; "table"; "library"; "book";
}

matcher open {
  // Referencing the matcher `Object`:
  "open the (Object)obj;" { return obj }
}

print( open.exec('open the book') )
[ 'book' ]
```

Dealing with multiple interpretations:

```Javascript
matcher M1 {
  "test" { yield "value" }
  "test" { yield "other value" }
  "test" if ( a == None ) { yield "a is None" }
  "nomatch" { yield "no value" }
}

a = 10;

result = M1.exec(test)
print(result)
[ "value", "other value" ]
// (it return a list with all possible matched returned values)
```

A complex example showing the flexibility of the system:

```Javascript
matcher Object {
  "duck"; "bird";
}

matcher Verb {
  "see"; "saw"; "cut";
}

matcher Sentence {
  "I (Verb)v; a (Object)obj;" {
    if (v == "saw")
      if (context.objects.saw != None) {
        // Parse it recursively:
        return Sentence.exec("I cut the %s" % obj)[0]
      } else {
        m = Sentence.exec("I see the %s" % obj)[0]
        m.past_tense = True;
        return m;
      }
    } else {
      m = map()
      m.verb = v; m.obj = obj;
      return m;
    }
  }
}

result = Sentence.exec("I saw a duck")
context = map()
context.objects = map()

print(result)
[ { "verb": "see", "obj": "duck", "past_tense": True } ]
```

