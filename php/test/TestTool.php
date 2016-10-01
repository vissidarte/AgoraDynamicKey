<?php

    function assertEqual($expect, $actual)
    {
        if ($expect != $actual) {
            echo("assert failed");
            echo("  expect:\n    " . $expect);
            echo("  actual:\n    " . $actual);
            echo("\n");
        } else {
            echo("assert ok\n");
            echo("\n");
        }
    }

?>