<?php
namespace QTranslate;
use \qtr;

/**
 * Notify the previous users of a verse that a new translation has been
 * submitted. Maybe they would like to review it and change their like.
 */
{
  $subject = "[$subject_prefix] " . t('New translation: !translation',
             ['!translation' => cut($params->new_translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);

  $body[] = t("A new translation has been suggested for a verse
that you have reviewed. If you think that the new
translation is better than the one that you have
supported, then you can support the new one at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $liked_translation = t('Supported Translation:');
  $new_translation = t('New Translation:');
  $body[] = "
    <hr/>

    <p style='direction:rtl; align:right; float:right; font-size:18px;'>
      <a href='$url' target='_blank'>$params->verse</a>
    </p>

    <h4 style='clear:both;'>$liked_translation</h4>
    <ul>
      <li>$params->liked_translation</li>
    </ul>

    <h4>$new_translation</h4>
    <ul>
      <li>$params->new_translation</li>
    </ul>
  ";
}
