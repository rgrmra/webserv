<?php

$name_post = filter_input(INPUT_POST, 'name', FILTER_SANITIZE_STRING);
$email_post = filter_input(INPUT_POST, 'email', FILTER_SANITIZE_EMAIL);
$phone_post = filter_input(INPUT_POST, 'phone', FILTER_SANITIZE_STRING);

echo "Hello, $name_post! We will send you an email at $email_post or call you at $phone_post.";

?>