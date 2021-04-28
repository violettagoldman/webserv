<html>
<body>

<?php
  // File: tiny.php
  // Example of a PHP script that produces static output
  // It can reside in any public_html directory and it
  // does not need to be executable

$username = "Susan";
echo "Hello " . $username.".";
echo "<br>";


// echo "Server software: " . $_ENV['SERVER_SOFTWARE'];
// echo $_SERVER;
print_r(get_defined_vars());

print_r($_SERVER);
// phpinfo();
?>

</body>
</html>