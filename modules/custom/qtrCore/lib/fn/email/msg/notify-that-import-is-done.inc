<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the user that the import is done.
 */
{
  $subject = "[$subject_prefix] "
    . t('Import of !filename', ['!filename' => $params->filename]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("The import of the file '!filename', that you have requested, is done.",
            ['!filename' => $params->filename]);
  $body[] = t('Thanks');

  if ($params->messages) {
    $output_messages = t('Output messages:');
    $body[] = "
      <hr/>
      <h4>$output_messages</h4>
      <xmp>$params->messages</xmp>
    ";
  }
}
