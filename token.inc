<?php
// $Id$

/**
 * Return the value of $original, with all instances of placeholder
 * tokens replaced by their proper values.
 *
 * @param original
 *  A string, or an array of strings, to perform token substitutions
 *  on.
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @param object
 *   Optionally, the object to use for building substitution values.
 *   A node, comment, user, etc.
 * @param leading
 *    Character(s) to prepend to the token key before searching for
 *    matches. Defaults to an open-bracket.
 * @param trailing
 *    Character(s) to append to the token key before searching for
 *    matches. Defaults to a close-bracket.
 * @return The modified version of $original, with all substitutions
 *   made.
 **/
function token_replace($original, $type = 'global', $object = NULL, $leading = '[', $trailing = ']') {
  $full = token_get_values($type, $object);
  $tokens = token_prepare_tokens($full->tokens, $leading, $trailing);
  $values = $full->values;
  $result = str_replace($tokens, $values, $original);
  return $result;
}

/**
 * Return a list of valid substitution tokens and their values for
 * the specified type.
 *
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @param object
 *   Optionally, the object to use for building substitution values.
 *   A node, comment, user, etc.
 * @return
 *   A keyed array containing the substitution tokens and the substition
 *   values for the passed-in type and object.
 */
function token_get_values($type = 'global', $object = NULL) {
  static $tokens;

  include_once(drupal_get_path('module', 'token') . '/token_node.inc');
  include_once(drupal_get_path('module', 'token') . '/token_user.inc');
  if (module_exists('content')) {
    include_once(drupal_get_path('module', 'token') . '/token_cck.inc');
  }

  if (!isset($tokens)) {
    $tokens = array();
  }

  $id = _token_get_id($type, $object);
  if (isset($tokens[$type][$id])) {
    $tmp_tokens = $tokens[$type][$id];
  }
  else {
    $tmp_tokens = module_invoke_all('token_values', $type, $object);
    $tokens[$type][$id] = $tmp_tokens;
  }

  // Special-case global tokens, as we always want to be able to process
  // those substitutions.
  if (!isset($tokens['global']['default'])) {
    $tokens['global']['default'] = module_invoke_all('token_values', 'global');
  }

  $all = array_merge($tokens['global']['default'], $tokens[$type][$id]);

  $result = new stdClass();
  $result->tokens = array_keys($all);
  $result->values = array_values($all);

  return $result;
}

/**
 * A helper function that retrieves all currently exposed tokens,
 * and merges them recursively. This is only necessary when building
 * the token listing -- during actual value replacement, only tokens
 * in a particular domain are requested and a normal array_marge() is
 * sufficient.
 *
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @return
 *   The array of usable tokens and their descriptions, organized by
 *   token type.
 */
function token_get_list($type = 'all') {
  $return = array();
  foreach (module_implements('token_list') as $module) {
    $function = $module .'_token_list';
    $result = $function($type);
    if (is_array($result)) {
      foreach ($result as $category => $tokens) {
        foreach ($tokens as $token => $title) {
          $return[$category][$token] = $title;
        }
      }
    }
  }
  return $return;
}

/**
 * A helper function that transforms all the elements of an
 * array. Used to change the delimiter style from brackets to
 * percent symbols etc.
 *
 * @param tokens
 *    The array of tokens keys with no delimiting chacaters
 * @param leading
 *    Character(s) to prepend to the token key before searching for
 *    matches. Defaults to an open-bracket.
 * @param trailing
 *    Character(s) to append to the token key before searching for
 *    matches. Defaults to a close-bracket.
 *  @return
 *    The array of token keys, each wrapped in the specified
 *    delimiter style.
 */
function token_prepare_tokens($tokens = array(), $leading = '[', $trailing = ']') {
  foreach ($tokens as $key => $value) {
    $tokens[$key] = $leading . $value . $trailing;
  }
  return $tokens;
}

// Internal utility function used for static caching.
function _token_get_id($type = 'global', $object = NULL) {
  if (!isset($object)) {
    return "default";
  }
  switch ($type) {
    case 'node':
      return $object->nid;
    case 'comment':
      return $object->cid;
    case 'user':
      return $user->uid;
    default:
      return md5(serialize($object));
  }
}