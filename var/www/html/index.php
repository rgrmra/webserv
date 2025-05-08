#!/usr/bin/php-cgi
<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Verifica se os campos 'username' e 'email' existem e exibe seus valores
    if (isset($_POST['username']) && isset($_POST['email'])) {
        echo "Username: " . htmlspecialchars($_POST['username']) . "<br>\n";
        echo "Email: " . htmlspecialchars($_POST['email']) . "<br>\n";
    } else {
        echo "Erro: Dados não recebidos corretamente.<br>\n";
    }
}
?>
