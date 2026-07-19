# Flourish - the better language for modern web developement
Tired of typing the same boilerplate over and over again? Of mismatching ending element blocks? Of completely screwing over tag names, attribute names, and worse? **Flourish** is the solution. Elegant, modern, and simple.

## Try it out
Go to our [Getting started](#getting-started "Getting started") portion to download Flourish. Once that's set up, create a file called `main.fsh` and paste this inside of it:
```
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
"
```
Then simply run `flourish ./ 3000` inside the directory you created `main.fsh` in. This creates a port on `localhost:3000` (`:8080` is the default when run without `3000`). It should display `main.fsh` as a web file.

## Getting started
Building from source (POSIX systems):
```
git clone https://github.com/zyrikl/flourish.git
make build
```
Even building from source is that easy!