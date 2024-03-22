
module NumericalMethods =

    // iteration method
    let rec solveByIterations (func : float -> float) (a : float) (b : float) (error : float) : float =
        let mid = (a + b) / 2.0
        if abs (b - a) < error then
            mid
        else
            let fa = func a
            let fmid = func mid
            if fa * fmid < 0.0 then
                solveByIterations func a mid error
            else
                solveByIterations func mid b error

    // dichotomy method
    let solveByDichotomy (func : float -> float) (a : float) (b : float) (error : float) : float =
        let rec dichotomyIter low high =
            let mid = (low + high) / 2.0
            if abs (high - low) < error then
                mid
            else
                let fmid = func mid
                if fmid = 0.0 then
                    mid
                elif fmid * func low < 0.0 then
                    dichotomyIter low mid
                else
                    dichotomyIter mid high
        dichotomyIter a b

    // Newton's method
    let solveByNewton (func : float -> float) (funcDerivative : float -> float) (initialGuess : float) (error : float) : float =
        let rec newtonIter x =
            let deltaX = func x / funcDerivative x
            let newX = x - deltaX
            if abs deltaX < error then
                newX
            else
                newtonIter newX
        newtonIter initialGuess

let equation1 x = sqrt(1.0 - 0.4 * x * x) - asin x
let equation2 x = exp x - exp (-x) - 2.0
let equation3 x = sin(log x) - cos(log x) + 2.0 * log x

let equation2_derivative x = exp x + exp (-x)
let equation3_derivative x = (1.0 / x) * cos(log x) - (1.0 / x) * sin(log x) - 2.0 / x


let solution1_iteration = NumericalMethods.solveByIterations equation1 0.0 1.0 0.0001
let solution2_newton = NumericalMethods.solveByNewton equation2 equation2_derivative 0.5 0.0001
let solution3_dichotomy = NumericalMethods.solveByDichotomy equation3 1.0 3.0 0.0001


printfn "Solution 1 (Iterations): %f" solution1_iteration
printfn "Solution 2 (Newton's Method): %f" solution2_newton
printfn "Solution 3 (Dichotomy): %f" solution3_dichotomy
