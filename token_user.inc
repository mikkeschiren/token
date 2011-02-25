<?php

/**
 * Implementation of hook_token_values()
 */
function user_token_values($type, $object = NULL) {
  switch ($type) {
    case 'user':
      if (isset($object)) {
        $account = $object;
      }
      else {
        global $user;
        $account = $user;
      }

      $values['user']           = $account->uid ? check_plain($user->name) : variable_get('anonymous', 'Anonymous');
      $values['user-raw']           = $account->uid ? $user->name : variable_get('anonymous', 'Anonymous');
      $values['uid']            = $account->uid;
      $values['mail']           = $account->uid ? $account->mail : '';
      $values['reg-date']       = $account->uid ? date('F j, Y', $account->created) : '';
      $values['reg-since']      = $account->uid ? format_interval($account->created) : '';
      $values['log-date']       = $account->uid ? date('F j, Y', $account->access) : '';
      $values['log-since']      = $account->uid ? format_interval($account->access) : '';
      $values['user-date']      = format_date(time(), 'short', '', $account->timezone);
      $values['user-edit-url']  = url('user/'. $account->uid .'/edit', NULL, NULL, TRUE);

      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function user_token_list($type = 'all') {
  if ($type == 'user' || $type == 'all') {
    $tokens['user']['user']           = t("User's name");
    $tokens['user']['user-raw']       = t("User's unfiltered name. WARNING - raw user input.");
    $tokens['user']['uid']            = t("User's ID");
    $tokens['user']['mail']           = t("User's email address");
 
    $tokens['user']['reg-date']       = t("User's registration date");
    $tokens['user']['reg-since']      = t("Days since the user registered");
    $tokens['user']['log-date']       = t("User's last login date");
    $tokens['user']['log-since']      = t("Days since the user's last login");
    $tokens['user']['user-date']      = t("The current date in the user's timezone");
    $tokens['user']['user-edit-url']  = t('Account edit URL');

    return $tokens;
  }
}