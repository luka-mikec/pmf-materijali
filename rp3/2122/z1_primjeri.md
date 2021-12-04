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
                            {"children", new node("Neki dugačak tekst   neki dugačak tekst")},
                        }
                    ),
                    new node(
                        new Dictionary<string, node> {
                            {"x", new node(18)},
                            {"y", new node(1)},
                            {"w", new node(20)},
                            {"h", new node(5)},
                            {"z", new node(1)},
                            {"children", new node("Neki još dulji tekst neki još dulji tekst neki još dulji tekst")},
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
            children: "Neki dugačak tekst   neki dugačak tekst",
        },
        {
            x: 18,
            y: 1,
            w: 20,
            h: 5,
            z: 1,
            children: "Neki još dulji tekst neki još dulji tekst neki još dulji tekst",
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
|                  |Neki još dulji    |    |/----\                        |    |
|                  |tekst neki još    |    ||    |                        |    |
|  /------------------\ji tekst neki  |    |\----/                  /----\|    |
|  |Neki dugačak tekst|---------------/    |                        |    ||    |
|  |neki dugačak tekst|                    |                        \----/|    |
|  \------------------/                    \------------------------------/    |
|                                                                              |
\------------------------------------------------------------------------------/
```



