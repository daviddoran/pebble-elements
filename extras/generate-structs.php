#!/usr/bin/env php
<?php

/**
 * Generate C structs from a JSON file of chemical elements
 *
 * To run with the default data:
 *      ./generate-structs.php
 *
 * To run with some other JSON file:
 *      ./generate-structs.php /path/to/some/other/file
 *
 * The C code is printed to stdout.
 */

$filename = __DIR__ . "/elements.json";

if ($argc > 1) {
    $filename = $argv[1];
}

try {
    echo array_to_structs(file_to_array($filename));
} catch (Exception $e) {
    die($e->getMessage() . "\n");
}

function file_to_array($filename) {
    if (!file_exists($filename)) {
        throw new Exception("File not found.");
    }
    return json_decode(file_get_contents($filename), false);
}

function array_to_structs(array $elements) {
    $buffer = "const Element elements[] = {\n";

    $buffer .= implode(", \n", array_map(function ($element) {
        return "\t{" . implode(", ", array(
            sprintf(".atomic = %d", $element->atomic),
            sprintf('.symbol = "%s"', $element->symbol),
            sprintf('.name = "%s"', $element->name)
        )) . "}";
    }, $elements));

    $buffer .= "\n};\n\n";
    $buffer .= sprintf("const uint8_t num_elements = %d;\n", count($elements));
    return $buffer;
}
