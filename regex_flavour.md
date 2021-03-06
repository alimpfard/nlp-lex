## Syntax

1. Syntax Elements

    + `\`        Escape (Enables or disables metachar)
    + `|`        Alternation
    + `(...)`    Group
    + `[...]`    Character Class
    + `{{...}}`  Rule embed
    + `\E{...}`  Embedded Code

2. Characters

    + `\t`           Horizontal Tab         (0x09)
    + `\v`           Vertical Tab           (0x0b)
    + `\n`           Newline (LF)           (0x0a)
    + `\r`           Carriage Return        (0x0d)
    + `\b`           Backspace              (0x08)
    + `\f`           Form Feed              (0x0c)
    + `\a`           Bell                   (0x07)
    + `\uHHHHHHHH`   Wide Unicode codepoint (Code point value)

3. Character Types

    1. Classes

        + `.`            Any character except newline
        + `\w`           Word character                (Letter|Mark|Number|Connector Punc)
        + `\W`           non-word character            (reverse of `\w`)
        + `\s`           Whitespace character          (Line Sep | Par Sep | Space Sep)
        + `\S`           non-whitespace character      (reverse of `\s`)
        + `\d`           Decimal digit character       (Number Decimal)
        + `\D`           non-digit character            (reverse of `\d`)

    2. Unicode Classes or properties

        + `\p{property-name}`   Unicode Class (L|Z|M|N|P) and their subcategories

4. Quantifiers

    1. Greedy

        + `?`        1 or 0 times
        + `*`        0 or more times
        + `+`        1 or more times
        + `{n,m}`    (n <= m) at least n but no more than m times
        + `{n,}`     at least n times
        + `{n}`      n times

    2. Reluctant

        + `*?`       0 or more times
        + `+?`       1 or more times


5. Assertions

    + `^`      beginning of line
    + `$`      end of line
    + `\b`     word boundary
    + `\B`     non-word boundary
    + `\A`     True beginning of string
    + `\G`     beginning of match
    + `\K`     set reported start of match

6. Character Classes (Note: Alternation of character classes merges them together)

    + `^...`    negative character class

7. Extended Groups (not yet implemented)

    + `(?:...)` Shy group (non-capturing)
    + `(?imIM:subexp)`   specify option for match of subexp (not a group)
        + `i`: ignore case
        + `m`: multiline
        + `I`: match case
        + `M`: non multiline

8. Backreferences (not yet implemented)

    + Explanation: Backreferencing a group means to re-match its actual matched value    
    + `\<n>`    backreference group number `n`

9. Subexpr calls (in testing)

    + Explanation: Subexpr calling attempts to match a given group *again*, and then continuing from the ending position (if the match was a success) or terminating and reverting to the closest valid match
    + `\g<n>`   attempt to call the `n`th group (leftmost calls are undefined)

10. Capture groups (indexing only)

    + `(...)` is a capture group and its index is the number of open-parens since the beginning of the regexp (unique)
    + `(?|...)` branch reset group: all alternative captures inside this group will start with the same group index
    + `(?#...)` inline comment: text inside this 'group' is ignored by the engine (the starting paren still increments the next capture index)
    
11. Embedded Rule Actions (WIP)
    + Appears as an escaped entity `\E{...}`, and acts on its own
    + Is run after the subexpression before it is successfully matched
    + Toplevel syntax:
        * `\E{ statement-list }` where 
            + `statement-list : expression ';' statement-list | `
            + `expression : ifexpr | loopexpr | wloopexpr | callexpr | number | opexpr`
            + `ifexpr : 'if' expression 'then' expression 'else' expression`
            + `loopexpr : 'for' identifier '=' expression ',' expression (',' expression)? 'in' expression`
            + `wloopexpr : 'while' expression 'in' expression`
            + `callexpr : identifier '(' arglist ')'`
            + `arglist : expression (',' arglist)? | `
            + `opexpr : operator expression | expression operator expression`
            + `operator : <defined single character>`


## Basic Features

| support         | '+' quantifier | Nested character classes | Non-greedy quantifiers | Non-capturing groups | Recursion      | Lookahead      | Lookbehind     | Backreferences | Indexable captures | Directives         | Conditionals       | Atomic Groups      | Named Captures     | Comments           | Embedded code      | Unicode Property   | Balancing Groups   | Variable length lookbehind |
| :-------------  | :------------- | :-------------           | :-------------         | :-------------       | :------------- | :------------- | :------------- | :------------- | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------             |
| Current support | Yes            | No                       | Yes                    | Yes                  | WIP              | No             | No             | No             | Yes                | No                 | No                 | Yes                | No                 | Yes                | Yes                | Partial            | No                 | No                         |
| Planned support | -              | No                       | -                      | -                    | Yes              | Yes            | Yes            | Yes            | -                  | Yes                | No                 | -                  | No                 | Yes                | WIP                | Yes                | No                 | No                         |


## API features

| support | UTF-16  | UTF-8   | Multiline | Partial match               |
| :------ | :------ | :------ | :------   | :------                     |
| Current | No      | Yes     | Yes       | Only if partial rule exists |
| Planned | No      | -       | -         | -                           |
