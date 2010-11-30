<?xml version="1.0"?>
<xsl:stylesheet version="2.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xs="http://www.w3.org/2001/XMLSchema">

<xsl:output method="text"/>

<xsl:template match="/">
  <xsl:text>{| border="1"</xsl:text><br/>
  <xsl:text>!Client||Duration||Task</xsl:text><br/>
  <xsl:for-each select="year/day/task">
    <xsl:sort select="@client"/>
    <xsl:text>|-</xsl:text><br/>
      <xsl:text>|</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>
      <xsl:variable name="duration" as="xs:dayTimeDuration" select="xs:dateTime("@start") - xs:dateTime("@end")"/>
      <xsl:value-of select="$duration"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@name"/><br/>
  </xsl:for-each>
  <xsl:text>|}</xsl:text>
</xsl:template>
</xsl:stylesheet>

