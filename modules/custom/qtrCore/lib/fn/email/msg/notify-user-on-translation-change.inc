<?php
namespace QTranslate;
use \qtr;

/**
 * Send an email notification to a user, when a translation that he has liked
 * for is modified.
 */
{
  $subject = "[$subject_prefix] "
    . t("Translation: !translation",
      ['!translation' => cut($params->new_translation, 30)]);

  $body = array();
  $body[] = t('Greetings !username,', ['!username' => $params->username]);
  $body[] = t("The following translation has been modified by its author.
You are being notified because you have supported the old
translation and your support now counts for the new version
of the translation.

If the new version of the translation is OK for you,
then there is nothing you should do. Otherwise, you
can change your support at this link:");
  $body[] = $url;
  $body[] = t('Thanks');

  $old_translation = t('Old Translation:');
  $new_translation = t('New Translation:');
  $body[] = "
    <hr/>

    <p style='direction:rtl; align:right; float:right; font-size:18px;'>
      <a href='$url' target='_blank'>$params->verse</a>
    </p>

    <h4 style='clear:both;'>$old_translation</h4>
    <ul>
      <li>$params->old_translation</li>
    </ul>

    <h4>$new_translation</h4>
    <ul>
      <li>$params->new_translation</li>
    </ul>
   ";
}
