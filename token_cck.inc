<?php
// $Id$


// Two helper functions that generate appropriate tokens for CCK-added fields.
function content_token_values($type, $object = NULL) {
  $tokens = array();
  if ($type == 'node') {
    $node = $object;
    content_view($node);

    $field_types = _content_field_types();

    foreach (content_fields() as $field) {
      
      $items = $node->$field['field_name'];
      if (!empty($items)) {
        $function = $field_types[$field['type']]['module'] . '_token_values';
        if (function_exists($function)) {
          $sub_list = $function('field', $items);
          foreach($sub_list as $token => $value) {
            $tokens[$field['field_name'] .'-'. $token] = $value;
          }
        }
      }
    }
  }
  return $tokens;
}

function content_token_list($type = 'all') {
  if ($type == 'node' || $token == 'all') {
    $list = array();
    $field_types = _content_field_types();

    foreach (content_fields() as $field) {
      $sub_list = array();
      $function = $field_types[$field['type']]['module'] . '_token_list';
      if (function_exists($function)) {
        $sub_list = $function('field');
        foreach ($sub_list as $category => $token_list) {
          foreach ($token_list as $token => $description) {
            $list['CCK '. $category][$field['field_name'] .'-'. $token] = $description;
          }
        }
      }
    }

    return $list;
  }
}



function nodereference_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();

    $tokens['node reference']['nid']   = t("Referenced node ID.");
    $tokens['node reference']['title'] = t("Referenced node title.");
    $tokens['node reference']['link']  = t("Formatted html link to the node.");

    return $tokens;
  }
}

function nodereference_token_values($type, $object = NULL) {
  if ($type == 'field') {
    $item = $object[0];

    $tokens['nid']   = $item['nid'];
    $tokens['title'] = strip_tags($item['nid']);
    $tokens['link']  = $item['view'];

    return $tokens;
  }
}



function number_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();

    $tokens['number']['raw']       = t("Raw number value.");
    $tokens['number']['formatted'] = t("Formatted number value.");

    return $tokens;
  }
}

function number_token_values($type, $object = NULL) {
  if ($type == 'field') {
    $item = $object[0];

    $tokens['raw']       = $item['value'];
    $tokens['formatted'] = $item['view'];

    return $tokens;
  }
}



function text_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();
    $tokens['node reference']['text'] = t("Text field contents.");
    return $tokens;
  }
}

function text_token_values($type, $object = NULL) {
  if ($type == 'field') {
    $item = $object[0];
    $tokens['text'] = $item['view'];
    return $tokens;
  }
}



function userreference_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();

    $tokens['user reference']['uid']   = t("Referenced user ID");
    $tokens['user reference']['name']  = t("Referenced user name");
    $tokens['user reference']['link']  = t("Formatted html link to referenced user");

    return $tokens;
  }
}

function userreference_token_values($type, $object = NULL) {
  if ($type == 'field') {
    $item = $object[0];

    $tokens['uid']   = $item['uid'];
    $tokens['name']  = strip_tags($item['view']);
    $tokens['link']  = $item['view'];

    return $tokens;
  }
}



function imagefield_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();

    $tokens['image field']['uid']   = t("Referenced user ID");
    $tokens['image field']['name']  = t("Referenced user name");
    $tokens['image field']['link']  = t("Formatted html link to referenced user");

    return $tokens;
  }
}