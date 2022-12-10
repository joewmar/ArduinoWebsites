<?php

    $locker= $_GET["lock"];

    require("database_connect.php");
    $sql = "";
    // Create connection
    if ($locker == 1){
        // Check connection
        $sql = "INSERT INTO tblstudlockerinfo (MessageHistory, DateHappen) VALUES ('OPENED', NOW())";
    }
    else if ($locker == 0){
        // Create connection
        $sql = "INSERT INTO tblstudlockerinfo (MessageHistory, DateHappen) VALUES ('CLOSED', NOW())";
    }

    // Check connection
    if ($con->connect_error) {
        die("Connection failed: " . $con->connect_error);
    }
    
    if ($con->query($sql) === TRUE) {
        echo 'Record added successfully';
    } else {
        echo 'Error added record: ' . $con->error;
    }
    $sql = "";
    require("database_close.php"); 
?>