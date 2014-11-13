<?php

class PumWebformExportCustomFieldHandler {
  
  public function __construct() {
    
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportCustomFieldHandler();
    $node->webform_civicrm = $e->checkCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->checkComponents($node->webform['components']);
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportCustomFieldHandler();
    $node->webform_civicrm = $e->importCheckCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->importCheckComponents($node->webform['components']);
  }
  
  public function checkComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (preg_match("/civicrm_\d+_.*_\d+_cg(\d+)_custom_(\d+)/", $element['form_key'])) {
        $element['form_key'] = $this->exportFormKey($element['form_key']);
      } elseif (preg_match("/(civicrm_\d+_contact_\d+_(.*)_custom_)(\d+)/", $element['form_key'])) {
        $element['form_key'] = $this->exportFormElementKey($element['form_key']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (preg_match("/civicrm_\d+_.*_\d+_cg#name#_(.*)_custom_#name#_(.*)/", $element['form_key'])) {
        $element['form_key'] = $this->importFormKey($element['form_key']);
      }  elseif (preg_match("/(civicrm_\d+_contact_\d+_(.*)_custom_)#cg_name#_(.*)_#cf_name#_(.*)/", $element['form_key'])) {
        $element['form_key'] = $this->importFormElementKey($element['form_key']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'number_of_cg#name#_') === 0) {
        $newKey = $this->importTranslateNumberOfKey($key);
      }
      
      if (is_array($element)) {
        $newElement = $this->importCheckCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  public function checkCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'number_of_cg') === 0) {
        $newKey = $this->exportTranslateNumberOfKey($key);
      }
      
      if (is_array($element)) {
        $newElement = $this->checkCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  protected function importTranslateNumberOfKey($key) {
    $custom_group_name = substr($key, strlen('number_of_cg#name#_'));
    $custom_group_id = $this->getCustomGroupId($custom_group_name);
    if ($custom_group_id) {
      return 'number_of_cg'.$custom_group_id;
    }
    return $key;
  }
  
  protected function exportTranslateNumberOfKey($key) {
    $custom_group_id = substr($key, strlen('number_of_cg'));
    $custom_group_name = $this->getCustomGroupName($custom_group_id);
    if ($custom_group_name) {
      return 'number_of_cg#name#_'.$custom_group_name;
    }
    return $key;
  }
  
  protected function exportFormKey($key) {
    $matches = array();
    preg_match("/(civicrm_\d+_.*_\d+_)cg(\d+)_custom_(\d+)/", $key, $matches);
    $custom_group_name = false;
    $custom_field_name = false;
    if (isset($matches[2])) {
      $custom_group_name = $this->getCustomGroupName($matches[2]);
    }
    if (isset($matches[2]) && isset($matches[3])) {
      $custom_field_name = $this->getCustomFieldName($matches[3]);
    }
    
    if ($custom_field_name && $custom_group_name) {
      return $matches[1].'cg#name#_'.$custom_group_name.'_custom_#name#_'.$custom_field_name;
    }
    return $key;
  }
  
  protected function exportFormElementKey($key) {
    $matches = array();
    preg_match("/(civicrm_\d+_contact_\d+_(.*)_custom_)(\d+)/", $key, $matches);
    $custom_field_name = false;
    if (isset($matches[3])) {
      $custom_field_name = $this->getCustomGroupFieldName($matches[3]);
    }
    
    if ($custom_field_name) {
      return $matches[1].$custom_field_name;
    }
    return $key;
  }
  
  protected function importFormElementKey($key) {
    $matches = array();
    preg_match("/(civicrm_\d+_contact_\d+_(.*)_custom_)#cg_name#_(.*)_#cf_name#_(.*)/", $key, $matches);
    $custom_group_id = false;
    $custom_field_id = false;
    if (isset($matches[3])) {
      $custom_group_id= $this->getCustomGroupId($matches[3]);
    }
    if ($custom_group_id && isset($matches[4])) {
      $custom_field_id = $this->getCustomFieldId($matches[4], $custom_group_id);
    }
    
    if ($custom_field_id && $custom_group_id) {
      return $matches[1].$custom_field_id;
    }
    return $key;
  }
  
  protected function importFormKey($key) {
    $matches = array();
    preg_match("/(civicrm_\d+_.*_\d+_)cg#name#_(.*)_custom_#name#_(.*)/", $key, $matches);
    $custom_group_id = false;
    $custom_field_id = false;
    if (isset($matches[2])) {
      $custom_group_id= $this->getCustomGroupId($matches[2]);
    }
    if ($custom_group_id && isset($matches[3])) {
      $custom_field_id = $this->getCustomFieldId($matches[3], $custom_group_id);
    }
    
    if ($custom_field_id && $custom_group_id) {
      return $matches[1].'cg'.$custom_group_id.'_custom_'.$custom_field_id;
    }
    return $key;
  }
  
  protected function getCustomGroupName($custom_group_id) {
    try {
      return civicrm_api3('CustomGroup', 'getvalue', array('id' => $custom_group_id, 'return' => 'name'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCustomGroupFieldName($custom_field_id) {
    try {
      $field = civicrm_api3('CustomField', 'getsingle', array('id' => $custom_field_id));
      $groupname = $this->getCustomGroupName($field['custom_group_id']);
      return '#cg_name#_'.$groupname.'_#cf_name#_'.$field['name'];
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCustomFieldName($custom_field_id) {
    try {
      return civicrm_api3('CustomField', 'getvalue', array('id' => $custom_field_id, 'return' => 'name'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCustomGroupId($custom_group_name) {
    try {
      return civicrm_api3('CustomGroup', 'getvalue', array('name' => $custom_group_name, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getCustomFieldId($custom_field_name, $custom_group_id) {
    try {
      return civicrm_api3('CustomField', 'getvalue', array('name' => $custom_field_name, 'custom_group_id' => $custom_group_id, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
}