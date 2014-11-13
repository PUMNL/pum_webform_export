<?php

class PumWebformExportLocationTypeHandler {
  
  protected $phone_type_id;
  
  protected $website_type_id;
  
  public function __construct() {
    $this->phone_type_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'phone_type', 'return' => 'id'));
    $this->website_type_id = civicrm_api3('OptionGroup', 'getvalue', array('name' => 'website_type', 'return' => 'id'));
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportLocationTypeHandler();
    $node->webform_civicrm = $e->checkCiviElements($node->webform_civicrm);
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportLocationTypeHandler();
    $node->webform_civicrm = $e->importCheckCiviElements($node->webform_civicrm);
  }
  
  public function importCheckCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'location_type_id') === 0) {
        $newElement = $this->getLocationTypeId($newElement);
      } elseif (strpos($key, 'phone_type_id') === 0) {
        $newElement = $this->getOptionId($newElement, $this->phone_type_id);
      } elseif (strpos($key, 'location_type_id') === 0) {
        $newElement = $this->getOptionId($newElement, $this->website_type_id);
      } elseif (is_array($element)) {
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
      if (strpos($key, 'location_type_id') === 0) {
        $newElement = $this->getLocationTypeName($newElement);
      } elseif (strpos($key, 'phone_type_id') === 0) {
        $newElement = $this->getOptionName($newElement, $this->phone_type_id);
      } elseif (strpos($key, 'location_type_id') === 0) {
        $newElement = $this->getOptionName($newElement, $this->website_type_id);
      } elseif (is_array($element)) {
        $newElement = $this->checkCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  protected function getLocationTypeName($loc_type_id) {
    try {
      return civicrm_api3('LocationType', 'getvalue', array('id' => $loc_type_id, 'return' => 'name'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $loc_type_id;
  }
  
  protected function getLocationTypeId($name) {
    try {
      return civicrm_api3('LocationType', 'getvalue', array('name' => $name, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $name;
  }
  
  protected function getOptionName($id, $option_group_id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('value' => $id, 'return' => 'name', 'option_group_id' => $option_group_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return $id;
  }
  
  protected function getOptionId($name, $option_group_id) {
    try {
      return civicrm_api3('OptionValue', 'getvalue', array('name' => $name, 'return' => 'value', 'option_group_id' => $option_group_id));
    } catch (Exception $ex) {
      //do nothing
    }
    return $name;
  }
  

  
}