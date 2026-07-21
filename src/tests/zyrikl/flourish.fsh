head {
    meta charset="utf-8"
    style "
        body {
            width: 500px;
            text-align: center;
            margin-left: auto;
            margin-right: auto;
        }
        p {
            font-variant-numeric: oldstyle-nums;
        }
        h1, h2, h3, h4, h5, h6 {
            font-variant: small-caps;
        }
        h1 {
            position: sticky;
            top: 0;
            background: white;
            padding: 10px;
            padding-top: 20px;
        }
        h2 {
            position: sticky;
            top: 60px;
            background: white;
        }
    "
    title "Zyrikl's Website"
}
body {
    h1 "Flourish - the better language for modern web developement"
    p {
        echo "Tired of typing the same boilerplate over and over again? Of mismatching ending element blocks? Of completely screwing over tag names, attribute names, and worse? "
        b "Flourish"
        echo " is the solution. Elegant, modern, and simple."

    h2 "Try it out"
    p {
        echo "Go to our "
        a href="#getting-started" "Getting started"
        echo " portion to download Flourish. Once that's set up, create a file called `main.fsh` and paste this inside of it:"
    }
    code '
body {
    div #first-project {
        h1 "My First Project"
        p "This is a Flourish project."
    }
}

// Styles
style "
    #first-project {
        background: blue;
        color: white;
        padding: 10px;
    }
"'

    p {
        echo "Website constructed using the "
        a href="./flourish.fsh" "Flourish programming language"
        echo ".<br/>Copyright &copy; 2026, Charles Wang"
    }
}