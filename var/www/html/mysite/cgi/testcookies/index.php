<?php


$MINUTE = 60;
$cookie_name = "cookiebom";
$cookie_content = "cookie é bom =X";
$cookie_expire = time() + ($MINUTE * 2);
$cookie_path = "/";



if(isset($_COOKIE[$cookie_name])) {
    echo "Cookie '" . $cookie_name . "' está definido!<br>";
    echo "Valor: " . $_COOKIE[$cookie_name] . "<br>";
} else {
    echo "Cookie '" . $cookie_name . "' não está definido!<br>";
    setcookie($cookie_name, $cookie_content, 
    $cookie_expire, $cookie_path);
}