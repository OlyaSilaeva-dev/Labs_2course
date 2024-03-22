let rec whileLoop condition action state =
    match condition state with
    | false -> state
    | true -> whileLoop condition action (action state)

let generateInterval a b n =
    let step = (b - a) / float (n - 1)
    [ for i in 0 .. n - 1 -> a + step * float i ]

let builtinFunc x =
    1.0 / (4.0 - x ** 4.0)

let naiveTaylorSeriesFunc x eps =
    let rec taylor n acc =
        let term = x ** (4.0 * float n) / (4.0 ** (float (n + 1)))
        if abs term < eps then
            n, acc
        else
            taylor (n + 1) (acc + term)
    taylor 0 0.0

let smartTaylorSeriesFunc x eps =
    let rec taylor n acc term =
        if abs term < eps then
            n, acc
        else
            taylor (n + 1) (acc + term) (x ** (4.0 * float n) / (4.0 ** (float (n + 1))))
    taylor 0 -1.0 1.0

let calculateAndPrint a b =
    printfn "x\t\tBuiltin\t\tSmart Taylor\t#terms\tNaive Taylor\t#terms"
    let n = 10
    let eps = 1e-8
    let interval = generateInterval a b n

    let rec loop x =
        if x > b then ()
        else
            let builtinVal = builtinFunc x
            let naiveTerms, naiveVal = naiveTaylorSeriesFunc x eps
            let smartTerms, smartVal = smartTaylorSeriesFunc x eps
            printfn "%f\t%f\t%f\t%d\t%f\t%d" x builtinVal smartVal smartTerms naiveVal naiveTerms
            loop (x + 0.1)
    
    loop a

calculateAndPrint 0.0 1.0
