## General rules

* Use a (to this project) consistent code style
* Write long, self-explaining function/variable names.
* If you add code from other people, don't forget to mention that in a function annotation or comment
* Document all functions with annotations (have a look at other functions)
* Comment your code (That's also useful for yourself, you will forget about things after some time)
* If you are working on a bigger feature, that will require multiple commits to be made, consider creating a new feature branch ("feature-yourfeaturename") and make a pull request when you are done to merge it with the master
* Write re-usable, capsulated, readable, clean code

## Checklist before submitting pull requests / committing new code

* Rules: read?
* Consistent code style
* Documentation written (function annotations, comments)
* Write a summary of what you did into the first commit message line, and more detailed information into the next lines.
* Avoid pushing merge commits (within the same branch), they pollute the histoy. Use "git rebase" to stack commits. [Here](http://randyfay.com/content/simpler-rebasing-avoiding-unintentional-merge-commits) is a good tutorial.

## Consistent code style

* We use tabs for intendation
* We use whitespace for aligning code in-line
* Use long, self-explaining variable/function names
* Don't use abbrevations anywhere in function/variable names etc., except they are well-known (like "HTTP")
* Use newlines to group your code into logical parts
