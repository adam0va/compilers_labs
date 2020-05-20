{
    "startNonTerminal": "PROGRAM",
    "terminals": [
        "begin",
        "end",
        ";",
        "=",
        "e"
    ],
    "nonTerminals": [
        "PROGRAM",
        "BLOCK",
        "LIST_OF_OPERATORS",
        "OPERATOR",
        "IDENTIFICATOR",
        "EXPRESSION",
        "LIST_OF_OPERATORS_"
    ],
    "rules": {
        "PROGRAM": [
            [
                "BLOCK"
            ]
        ],
        "BLOCK": [
            [
                "begin",
                "LIST_OF_OPERATORS",
                "end"
            ]
        ],
        "LIST_OF_OPERATORS": [
            [
                "OPERATOR",
                "LIST_OF_OPERATORS_"
            ]
        ],
        "OPERATOR": [
            [
                "IDENTIFICATOR",
                "=",
                "EXPRESSION"
            ]
        ],
        "IDENTIFICATOR": [],
        "EXPRESSION": [],
        "LIST_OF_OPERATORS_": [
            [
                ";",
                "OPERATOR",
                "LIST_OF_OPERATORS_"
            ],
            [
                "e"
            ]
        ]
    }
}