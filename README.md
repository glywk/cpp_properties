cpp_properties
==============

> This library provides a complete lexer to produce a property abstract syntax tree
according to the java properties grammar (https://docs.oracle.com/javase/8/docs/api/java/util/Properties.html).

Overview
--------
Example directory shows how to parse following valid java property file in different structures:
* The [simple_reader.cc](example/simple_reader/simple_reader.cc) example loads a property file in a collection of key/value.
* The [properties_tree.cc](example/properties_tree/properties_tree.cc) example loads a property file in a [boost::property_tree](https://www.boost.org/doc/libs/1_70_0/doc/html/property_tree.html).

<pre>  
#==============================================================================
# Copyright (c) 2015-2018 glywk
# https://github.com/glywk
# 
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#==============================================================================

# This file aim to demonstrate ail crazy properties definition allowed by the
# java properties format details can be found at
# https://docs.oracle.com/javase/8/docs/api/java/util/Properties.html

# =============================================================================
# The following part illustrates many allowed comment syntax
# =============================================================================

! -----------------------------------------------------------------------------
  # Comment line can start with '#'
  ! Comment line can start with '!'
  # Comment cannot use breaking line escape sequence \
    comment.break.line.should.not.hide.this.key1
  ! Comment cannot use breaking line escape sequence \
    comment.break.line.should.not.hide.this.key2
! -----------------------------------------------------------------------------

# =============================================================================
# The following part illustrates many allowed key syntax
# =============================================================================

! -----------------------------------------------------------------------------
! Simple key
! -----------------------------------------------------------------------------
simple

! -----------------------------------------------------------------------------
! Prefix / postfix key blanks are trimmed
! -----------------------------------------------------------------------------
  trim.keyl 
  trim.key2  value 2
  trim.key3: 
  trim.key3: value 3
  trim.key4= 
  trim.key5= value 5

! -----------------------------------------------------------------------------
! Break line space are ignored in key construction
! -----------------------------------------------------------------------------
no.\
   blank.\
         in.\
            this.\
          \&quot;\
        key\
      \&quot;\
.

! -----------------------------------------------------------------------------
! Break line in key with comment emulation
! -----------------------------------------------------------------------------
following.lines.includes.comment.syntax.in.a.key\
!--exclamation_comment_style--\
#--sharp_comment_style--\

! -----------------------------------------------------------------------------
! Simple key with escape chars
! -----------------------------------------------------------------------------
simple\ \=&gt;\ escaped\:\
             {\\t\=&gt;[\t]}\
             {\\n\=&gt;[\n]}\
             {\\r\=&gt;[\r]}\
             {\\f\=&gt;[\f]},\
           \ not_escaped\:\
             {\\\\\=&gt;[\\]}\
             {\\0\=&gt;[\0]}\
             {\\'\=,[\']}\
             {\\&quot;\=&gt;[\&quot;]}\
             {\\/\=&gt;[\/]}\
             {\\v\=&gt;[\v]}\
             {\\a\=&gt;[\a]}\
             {\\b\=&gt;[\b]}\
             {\\127\=&gt;[\127]}

! -----------------------------------------------------------------------------
! Simple key with iso 8859-1 specific chars
! -----------------------------------------------------------------------------
latin1.key.\\u00E9.expect.display(é).as.(\u00E9)
latin1.key.\\u00E9.expect.display.escaped(\é).as.(\u00E9) 
 
! -----------------------------------------------------------------------------
! Simple key with unicode chars
! -----------------------------------------------------------------------------
simple.unicode.char_16.smileys\ \u263a[\\u263a],\u263B[\\u263B]
simple.unicode.char_16.smi1eys.and.chars\ \u263a0[\\u263a]0,\u263B0[\\u263B]0
simple.unicode.escape.is.case.sensitive\ \U263a[\\U263a],\U263B[\\U263B]

# =============================================================================
# The following part illustrates many allowed separator syntax
# =============================================================================

! -----------------------------------------------------------------------------
! Key-value separated by blank chars
! -----------------------------------------------------------------------------
blank.separatedl	valuel
blank.separated2	\
                	value2
blank.separated3	value3

! -----------------------------------------------------------------------------
! Key-value separated by colon char
! -----------------------------------------------------------------------------
colon.separatedl:valuel
colon.separated2 :value2
colon.separated3 : value3
colon.separated4\
                 :value4
colon.separated5\
:\
value5
colon.separated6::value6.include.colon:
colon.separated7:=value7.include.equals=
colon.separated8 \
:=: value8.include.separator.chars :=
:

! -----------------------------------------------------------------------------
! Key-value separated by equal char
! -----------------------------------------------------------------------------
equal.separatedl=valuel
equal.separated2 =value2 
equal.separated3 = value3
equal.separated4\
                =value4
equal.separated5\
=\
value5
equal.separated6=:value6.include.colon:
equal.separated7==value7.include.equals=
equal.separated8 \
=:= value8.include.separator.chars =:
=

# =============================================================================
# The following part illustrates many allowed value syntax
# =============================================================================

! -----------------------------------------------------------------------------
! The most simple value (empty)
! -----------------------------------------------------------------------------
empty.key1
empty.key2:
empty.key3=

! -----------------------------------------------------------------------------
! Beginning blank value is trimmed
! -----------------------------------------------------------------------------
blank.key1
blank.key2  value 2
blank.key3:
blank.key3: value 3
blank.key4=
blank.key5= value 5

! -----------------------------------------------------------------------------
! Break line space are ignored in value construction
! -----------------------------------------------------------------------------
broken.line.in.value=\
no.\
   blank.\
         in.\
            this.\
          \&quot;\
      value\
    \&quot;\
.

! -----------------------------------------------------------------------------
! Break line in value with comment emulation
! -----------------------------------------------------------------------------
following.lines.includes.comment.syntax.in.a.value \
!--exclamation_comment_style--\
#--sharp_comment style--\

! -----------------------------------------------------------------------------
! Value with escape chars
! -----------------------------------------------------------------------------
escape.value \=&gt; escaped:\
             {\\t\=&gt;[\t]}\
             {\\n\=&gt;[\n]}\
             {\\r\=&gt;[\r]}\
             {\\f\=&gt;[\f]}, \
             not_escaped:\
             {\\\\\=&gt;[\\]}\
             {\\0\=&gt;[\0]}\
             {\\'\=&gt;[\']}\
             {\\&quot;\=&gt;[\&quot;]}\
             {\\/\=&gt;[\/]}\
             {\\v\=&gt;[\v]}\
             {\\a\=&gt;[\a]}\
             {\\b\=&gt;[\b]}\
             {\\l27\=&gt;[\127]}

! -----------------------------------------------------------------------------
! Simple value with iso 8859-1 specific chars
! -----------------------------------------------------------------------------
latin1.value \\u00E9.expect.display(é).as.(\u00E9)
latin1.value \\u00E9.expect.display.escaped(\é).as.(\u00E9) 

! -----------------------------------------------------------------------------
! Simple value with unicode chars
! -----------------------------------------------------------------------------
value.unicode.char_16.smileys \u263a[\\u263a],\u263B[\\u263B]
value.unicode.char_16.smileys.and.chars \u263a0[\\u263a]0,\u263B0[\\u263B]0
value.unicode.escape.is.case.sensitive \U263a[\\U263a],\U263B[\\U263B]

# =============================================================================
# The following part illustrates key syntax without end of line
# =============================================================================

! -----------------------------------------------------------------------------
! Key without end of line
! -----------------------------------------------------------------------------
single.key
</pre>
have to be understand as:
<pre> 
comment.break.line.should.not.hide.this.key1=
comment.break.line.should.not.hide.this.key2=
simple=
trim.keyl=
trim.key2=value 2
trim.key3= 
trim.key3= value 3
trim.key4= 
trim.key5= value 5
no.blank.in.this.&quot;key&quot;.=
following.lines.includes.comment.syntax.in.a.key!--exclamation_comment_style--#--sharp_comment_style--=
simple =&gt; escaped:{\t=&gt;[	]}{\n=&gt;[
]}{\r=&gt;[
]}{\f=&gt;[]}, not_escaped:{\\=&gt;[\]}{\0=&gt;[0]}{\'=,[']}{\&quot;=&gt;[&quot;]}{\/=&gt;[/]}{\v=&gt;[v]}{\a=&gt;[a]}{\b=&gt;[b]}{\127=&gt;[127]}=
latin1.key.\u00E9.expect.display(é).as.(é)=
latin1.key.\u00E9.expect.display.escaped(é).as.(é)=
simple.unicode.char_16.smileys ☺[\u263a],☻[\u263B]=
simple.unicode.char_16.smi1eys.and.chars ☺0[\u263a]0,☻0[\u263B]0=
simple.unicode.escape.is.case.sensitive U263a[\U263a],U263B[\U263B]=
blank.separatedl=valuel
blank.separated2=value2
blank.separated3=value3
colon.separatedl=valuel
colon.separated2=value2
colon.separated3= value3
colon.separated4=value4
colon.separated5=value5
colon.separated6=:value6.include.colon:
colon.separated7==value7.include.equals=
colon.separated8==: value8.include.separator.chars :=
=
equal.separatedl=valuel
equal.separated2=value2 
equal.separated3= value3
equal.separated4=value4
equal.separated5=value5
equal.separated6=:value6.include.colon:
equal.separated7==value7.include.equals=
equal.separated8=:= value8.include.separator.chars =:
=
empty.key1=
empty.key2=
empty.key3=
blank.key1=
blank.key2=value 2
blank.key3=
blank.key3= value 3
blank.key4=
blank.key5= value 5
broken.line.in.value=no.blank.in.this.&quot;value&quot;.
following.lines.includes.comment.syntax.in.a.value=!--exclamation_comment_style--#--sharp_comment style--
escape.value==&gt; escaped:{\t=&gt;[	]}{\n=&gt;[
]}{\r=&gt;[
]}{\f=&gt;[]}, not_escaped:{\\=&gt;[\]}{\0=&gt;[0]}{\'=&gt;[']}{\&quot;=&gt;[&quot;]}{\/=&gt;[/]}{\v=&gt;[v]}{\a=&gt;[a]}{\b=&gt;[b]}{\l27=&gt;[127]}
latin1.value=\u00E9.expect.display(é).as.(é)
latin1.value=\u00E9.expect.display.escaped(é).as.(é) 
value.unicode.char_16.smileys=☺[\u263a],☻[\u263B]
value.unicode.char_16.smileys.and.chars=☺0[\u263a]0,☻0[\u263B]0
value.unicode.escape.is.case.sensitive=U263a[\U263a],U263B[\U263B]
single.key=
</pre>

Compiling
---------

cmake supports 'out of source' builds by default, to use it do this:

```shell
mkdir build
cd build
cmake ..
make
```

this should create everything into the `build/` directory

Test
----

Run the cpp_properties test suites. This is covered by cmake:

```shell
make test
```
