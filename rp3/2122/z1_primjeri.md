## Primjer 1

```csharp

node x = new node(
    new Dictionary<string, node> {
        {
            "children", 
            new node(
                new List<node> {
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(2)},
                            {"y", new node(4)},
                            {"w", new node(20)},
                            {"h", new node(4)},
                            {"z", new node(2)},
                            {"children", new node("Neki dugacak tekst   neki dugacak tekst")},
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(18)},
                            {"y", new node(1)},
                            {"w", new node(20)},
                            {"h", new node(5)},
                            {"z", new node(1)},
                            {"children", new node("Neki jos dulji tekst neki jos dulji tekst neki jos dulji tekst")},
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(42)},
                            {"y", new node(1)},
                            {"w", new node(32)},
                            {"h", new node(7)},
                            {
                                "children", 
                                new node(
                                    new List<node> {
                                        new node(
                                                new Dictionary<string, node> {
                                                {"w", new node(6)},
                                                {"h", new node(3)},
                                                {"children", new node("")},
                                            }
                                        ),
                                        new node(
                                            new Dictionary<string, node> {
                                                {"x", new node(24)},
                                                {"y", new node(2)},
                                                {"children", new node(new List<node>() {})},
                                            }
                                        ),
                                    }
                                )
                            },
                        }
                    ),
                }
            )
        }
    }
);
    
// Prvi dio zadatka
Console.WriteLine(x.pretty_print());

// Drugi dio zadatka
Console.WriteLine(x);

```

Očekivani izlaz prvog dijela zadatka:

```
{
    children: [
        {
            x: 2,
            y: 4,
            w: 20,
            h: 4,
            z: 2,
            children: "Neki dugacak tekst   neki dugacak tekst",
        },
        {
            x: 18,
            y: 1,
            w: 20,
            h: 5,
            z: 1,
            children: "Neki jos dulji tekst neki jos dulji tekst neki jos dulji tekst",
        },
        {
            x: 42,
            y: 1,
            w: 32,
            h: 7,
            children: [
                {
                    w: 6,
                    h: 3,
                    children: "",
                },
                {
                    x: 24,
                    y: 2,
                    children: [],
                },
            ],
        },
    ],
}
```

Očekivani izlaz drugog dijela zadatka:

```
/------------------------------------------------------------------------------\
|                                                                              |
|                  /------------------\    /------------------------------\    |
|                  |Neki jos dulji    |    |/----\                        |    |
|                  |tekst neki jos    |    ||    |                        |    |
|  /------------------\ji tekst neki  |    |\----/                  /----\|    |
|  |Neki dugacak tekst|---------------/    |                        |    ||    |
|  |neki dugacak tekst|                    |                        \----/|    |
|  \------------------/                    \------------------------------/    |
|                                                                              |
\------------------------------------------------------------------------------/
```












## Primjer 2

```csharp

node x = new node(
    new Dictionary<string, node> {
        {"x", new node(20)},
        {"y", new node(3)},
        {"w", new node(30)},
        {"h", new node(123456789)},
        {"z", new node(-1234)},
        {
            "children", 
            new node(
                new List<node> {
                    new node(
                        new Dictionary<string, node> {
                            {"w", new node(2)},
                            {"h", new node(2)},
                            {"z", new node(-12345)},
                            {"children", new node("Prozor prekriven roditeljem")},
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(3)},
                            {"y", new node(2)},
                            {"w", new node(9)},
                            {"h", new node(4)},
                            {"children", new node("Vidljiv prozor")},
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(15)},
                            {
                                "children",                                    
                                new node(
                                    new List<node> {
                                        new node(
                                            new Dictionary<string, node> {
                                                {"z", new node(2)},
                                                {"children", new node("Jos jedan nevidljiv")}
                                            }
                                        ),
                                        new node(
                                            new Dictionary<string, node> {
                                                {"z", new node(3)},
                                                {"children", new node("Jos jedan vidljiv")}
                                            }
                                        )
                                    }
                                )
                            }
                        }
                    ),
                }
            )
        }
    }
);
    
// Prvi dio zadatka
Console.WriteLine(x.pretty_print());

// Drugi dio zadatka
Console.WriteLine(x);

```

Očekivani izlaz prvog dijela zadatka:

```
{
    x: 20,
    y: 3,
    w: 30,
    h: 123456789,
    z: -1234,
    children: [
        {
            w: 2,
            h: 2,
            z: -12345,
            children: "Prozor prekriven roditeljem",
        },
        {
            x: 3,
            y: 2,
            w: 9,
            h: 4,
            children: "Vidljiv prozor",
        },
        {
            x: 15,
            children: [
                {
                    z: 2,
                    children: "Jos jedan nevidljiv",
                },
                {
                    z: 3,
                    children: "Jos jedan vidljiv",
                },
            ],
        },
    ],
}
```

Očekivani izlaz drugog dijela zadatka:

```
/----------------------------\
|               /-----------\|
|               |/---------\||
|   /-------\   ||Jos jedan|||
|   |Vidljiv|   ||vidljiv  |||
|   |prozor |   ||         |||
|   \-------/   ||         |||
|               ||         |||
|               |\---------/||
|               \-----------/|
\----------------------------/
```








## Primjer 3

```csharp

node x = new node(
    new Dictionary<string, node> {
        {
            "children", 
            new node(
                new List<node> {
                    new node(
                        new Dictionary<string, node> {
                            {"z", new node(-1)},
                            {
                                "children", 
                                new node(
                                    new List<node> {
                                        new node(
                                            new Dictionary<string, node> {
                                                {"x", new node(2)},
                                                {"y", new node(2)},
                                                {"w", new node(25)},
                                                {"h", new node(4)},
                                                {"children", new node("Vidljivo dijete nevidljivog roditelja")},
                                            }
                                        ),
                                    }
                                )
                            }
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(27)},
                            {"h", new node(5)},
                            {"z", new node(1)},
                            {"children", new node("Lorem ipsum dolor sit amet consectetur adipiscing elit Nam hendrerit nisi sed sollicitudin pellentesque Nunc posuere purus rhoncus pulvinar aliquam")},
                        }
                    ),
                }
            )
        }
    }
);
    
// Prvi dio zadatka
Console.WriteLine(x.pretty_print());

// Drugi dio zadatka
Console.WriteLine(x);

```

Očekivani izlaz prvog dijela zadatka:

```
{
    children: [
        {
            z: -1,
            children: [
                {
                    x: 2,
                    y: 2,
                    w: 25,
                    h: 4,
                    children: "Vidljivo dijete nevidljivog roditelja",
                },
            ],
        },
        {
            x: 27,
            h: 5,
            z: 1,
            children: "Lorem ipsum dolor sit amet consectetur adipiscing elit Nam hendrerit nisi sed sollicitudin pellentesque Nunc posuere purus rhoncus pulvinar aliquam",
        },
    ],
}
```

Očekivani izlaz drugog dijela zadatka:

```
/------------------------------------------------------------------------------\
|                           /-------------------------------------------------\|
|                           |Lorem ipsum dolor sit amet consectetur adipiscing||
|                           |elit Nam hendrerit nisi sed sollicitudin         ||
|   /-----------------------|pellentesque Nunc posuere purus rhoncus pulvinar ||
|   |Vidljivo dijete        \-------------------------------------------------/|
|   |nevidljivog roditelja  |                                                  |
|   \-----------------------/                                                  |
|                                                                              |
|                                                                              |
\------------------------------------------------------------------------------/
```


