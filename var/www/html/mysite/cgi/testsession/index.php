<?php

session_start();


if (isset($_SESSION['username']) && isset($_SESSION['password']))
{
    echo "Welcome back, " . $_SESSION['username'] . "<br>";
    echo "Your password is: " . $_SESSION['password'] . "<br>";
    session_destroy();
}
else
{
    echo "Testing session variables<br>";
    $_SESSION['username'] = 'Webmaster';
    $_SESSION['password'] = 'password';
}

$cookie_name = "cookiebom";

if(isset($_COOKIE[$cookie_name])) {
    echo "Cookie '" . $cookie_name . "' está definido!<br>";
    echo "Valor: " . $_COOKIE[$cookie_name] . "<br>";
}

?>